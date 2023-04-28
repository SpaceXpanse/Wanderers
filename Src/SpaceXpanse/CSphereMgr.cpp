// Copyright (c) Martin Schweiger
// Licensed under the MIT License

// =======================================================================
// CSphereMgr: Rendering of the celestial sphere background at variable
// resolutions.
// =======================================================================

#include "D3dmath.h"
#include "D3d7util.h"
#include "SpaceXpanse.h"
#include "Scene.h"
#include "VPlanet.h"
#include "CSphereMgr.h"
#include "Texture.h"
#include "Camera.h"
#include "Log.h"
#include "OGraphics.h"

// =======================================================================
// Externals

extern SpaceXpanse *g_pSpaceXpanse;
extern Camera *g_camera;
extern TextureManager2 *g_texmanager2;
extern int patchidx[9];
extern DWORD VB_MemFlag; // dodgy
extern VBMESH *PATCH_TPL[14];
extern int NLAT[9], *NLNG[9];
extern LPDIRECT3DVERTEXBUFFER7 bbtarget; // dodgy
extern char DBG_MSG[256];

void ApplyPatchTextureCoordinates (VBMESH &mesh, LPDIRECT3DVERTEXBUFFER7 vtx, const TEXCRDRANGE &range);

static int ntot, nrad, nrender;

DWORD CSphereManager::vpX0, CSphereManager::vpX1, CSphereManager::vpY0, CSphereManager::vpY1;
double CSphereManager::diagscale;

// =======================================================================
// =======================================================================
// Class CSphereManager

CSphereManager::CSphereManager ()
{
	char *c = g_pSpaceXpanse->Cfg()->CfgVisualPrm.CSphereBgPath;
	if (!c[0]) {
		disabled = true;
		return;
	} else {
		strncpy (texname, c, 64);
		disabled = false;
	}

	intensity = (float)g_pSpaceXpanse->Cfg()->CfgVisualPrm.CSphereBgIntens;

	maxlvl = 8; // g_pSpaceXpanse->Cfg()->CSphereMaxLevel;
	maxbaselvl = min (8, maxlvl);
	int maxidx = patchidx[maxbaselvl];
	bPreloadTile = (g_pSpaceXpanse->Cfg()->CfgPRenderPrm.PreloadMode > 0);
	nhitex = nhispec = 0;
	diagscale = 0.0;

	tiledesc = new TILEDESC[maxidx]; TRACENEW
	memset (tiledesc, 0, maxidx*sizeof(TILEDESC));

	LoadPatchData ();
	LoadTileData ();
	LoadTextures ();

	Matrix R(2000,0,0, 0,2000,0, 0,0,2000);

	// rotation from galactic to ecliptic frame
	double theta = 60.25*RAD; // 60.18*RAD;
	double phi = 90.09*RAD; // 90.02*RAD;
	double lambda = 173.64*RAD; // 173.6*RAD;
	double sint = sin(theta), cost = cos(theta);
	double sinp = sin(phi), cosp = cos(phi);
	double sinl = sin(lambda), cosl = cos(lambda);
	ecl2gal.Set (cosp,0,sinp, 0,1,0, -sinp,0,cosp);
	ecl2gal.premul (Matrix (1,0,0, 0,cost,sint, 0,-sint,cost));
	ecl2gal.premul (Matrix (cosl,0,sinl, 0,1,0, -sinl,0,cosl));
	R.premul (ecl2gal);

	trans = _M(R.m11,R.m12,R.m13,0,
		       R.m21,R.m22,R.m23,0,
               R.m31,R.m32,R.m33,0,
			   0,    0,    0,    1);
}

// =======================================================================

CSphereManager::~CSphereManager ()
{
	if (disabled) return;

	DWORD i, maxidx = patchidx[maxbaselvl];
	DWORD counter = 0;

	if (ntex) {
		for (i = 0; i < ntex; i++) {
			texbuf[i]->Release();
			if (!(++counter % 100))
				g_pSpaceXpanse->UpdateDeallocationProgress();
		}
		delete []texbuf;
	}

	for (i = 0; i < maxidx; i++) {
		if (tiledesc[i].vtx) tiledesc[i].vtx->Release();
		if (!(++counter % 100))
			g_pSpaceXpanse->UpdateDeallocationProgress();
	}
	delete []tiledesc;	

	g_pSpaceXpanse->UpdateDeallocationProgress();
}

// =======================================================================

void CSphereManager::CreateDeviceObjects (LPDIRECT3D7 d3d, LPDIRECT3DDEVICE7 dev)
{
	D3DVIEWPORT7 vp;
	dev->GetViewport (&vp);
	vpX0 = vp.dwX, vpX1 = vpX0 + vp.dwWidth;
	vpY0 = vp.dwY, vpY1 = vpY0 + vp.dwHeight;
	// viewport size for clipping calculations

	diagscale = (double)vp.dwWidth/(double)vp.dwHeight;
	diagscale = sqrt(1.0 + diagscale*diagscale);
}

// =======================================================================

void CSphereManager::DestroyDeviceObjects ()
{
}

// =======================================================================

bool CSphereManager::LoadPatchData ()
{
	// OBSOLETE

	int i;
	for (i = 0; i < patchidx[maxbaselvl]; i++)
		tiledesc[i].flag = 1;
	return false;
}

// =======================================================================

bool CSphereManager::LoadTileData ()
{
	if (maxlvl <= 8) // no tile data required
		return false;

	// TODO
	return true;
}

// =======================================================================

void CSphereManager::LoadTextures ()
{
	int i;

	// pre-load level 1-8 textures
	ntex = patchidx[maxbaselvl];
	texbuf = new LPDIRECTDRAWSURFACE7[ntex]; TRACENEW
	if (ntex = g_texmanager2->OpenTextures (texname, ".tex", texbuf, ntex)) {
		while (ntex < patchidx[maxbaselvl]) maxlvl = --maxbaselvl;
		while (ntex > patchidx[maxbaselvl]) texbuf[--ntex]->Release();
		// not enough textures loaded for requested resolution level
		for (i = 0; i < patchidx[maxbaselvl]; i++)
			tiledesc[i].tex = texbuf[i];
	} else {
		delete []texbuf;
		texbuf = 0;
		// no textures at all!
	}

	//  pre-load highres tile textures
	if (bPreloadTile && nhitex) {
		//TILEDESC *tile8 = tiledesc + patchidx[7];
		//PreloadTileTextures (tile8, nhitex, nhispec);
	}
}

// =======================================================================

void CSphereManager::Render (LPDIRECT3DDEVICE7 dev, int level, int bglvl)
{

	if (disabled) return;
	ntot = nrad = nrender = 0;
	static MATRIX4 Rsouth = {1,0,0,0,   0,-1,0,0,   0,0,-1,0,   0,0,0,1};

	float intens = intensity;
	if (bglvl) {
		intens *= (float)exp(-bglvl*0.05);
	}

	if (!intens) return; // sanity check

	level = min (level, maxlvl);

	RenderParam.dev = dev;
	RenderParam.tgtlvl = level;
	RenderParam.dwmat = trans;

	// build a transformation matrix for frustum testing
	MATRIX4 Mproj = g_camera->ProjectionMatrix();
	Mproj.m33 = 1.0; Mproj.m43 = -1.0;  // adjust near plane to 1, far plane to infinity
	MATRIX4 Mview = g_camera->ViewMatrix();
	RenderParam.viewproj = mul(Mview,Mproj);

	RenderParam.viewap = atan(diagscale * g_camera->TanAperture());

	int startlvl = min (level, 8);
	int hemisp, ilat, ilng, idx;
	int  nlat = NLAT[startlvl];
	int *nlng = NLNG[startlvl];
	int texofs = patchidx[startlvl-1];
	TILEDESC *td = tiledesc + texofs;
	TEXCRDRANGE range = {0,1,0,1};
	tilebuf = TileManager::tilebuf;

	Matrix rcam = g_camera->GRot();
	rcam.premul (ecl2gal);
	RenderParam.camdir.Set (rcam.m13, rcam.m23, rcam.m33);

	dev->SetRenderState (D3DRENDERSTATE_LIGHTING, FALSE);
	dev->SetRenderState (D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);
	dev->SetRenderState (D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
	dev->SetRenderState (D3DRENDERSTATE_CULLMODE, D3DCULL_CW);
	dev->SetTextureStageState (0, D3DTSS_ADDRESS, D3DTADDRESS_CLAMP);

	if (intens < 1.0f) {
		dev->SetRenderState (D3DRENDERSTATE_TEXTUREFACTOR, D3DRGBA(0,0,0,intens));
		dev->SetTextureStageState (0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		dev->SetTextureStageState (0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	}

	WaitForSingleObject (tilebuf->hQueueMutex, INFINITE);
	for (hemisp = idx = 0; hemisp < 2; hemisp++) {
		if (hemisp) { // flip world transformation to southern hemisphere
			RenderParam.dwmat = mul(Rsouth, RenderParam.dwmat);
		}
		for (ilat = nlat-1; ilat >= 0; ilat--) {
			for (ilng = 0; ilng < nlng[ilat]; ilng++) {
				ProcessTile (startlvl, hemisp, ilat, nlat, ilng, nlng[ilat], td+idx, 
					range, td[idx].tex, td[idx].ltex, td[idx].flag,
					range, td[idx].tex, td[idx].ltex, td[idx].flag);
				idx++;
			}
		}
	}
	ReleaseMutex (tilebuf->hQueueMutex);

	dev->SetRenderState (D3DRENDERSTATE_LIGHTING, TRUE);
	dev->SetRenderState (D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
	dev->SetRenderState (D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
	dev->SetRenderState (D3DRENDERSTATE_CULLMODE, D3DCULL_CCW);
	dev->SetTextureStageState (0, D3DTSS_ADDRESS, D3DTADDRESS_WRAP);

	if (intens < 1.0f) {
		dev->SetTextureStageState (0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		dev->SetTextureStageState (0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	}

	//sprintf (DBG_MSG, "total: %d, after radius culling: %d, after in-view culling: %d", ntot, nrad, nrender);
}

// =======================================================================

void CSphereManager::ProcessTile (int lvl, int hemisp, int ilat, int nlat, int ilng, int nlng, TILEDESC *tile,
	const TEXCRDRANGE &range, LPDIRECTDRAWSURFACE7 tex, LPDIRECTDRAWSURFACE7 ltex, DWORD flag,
	const TEXCRDRANGE &bkp_range, LPDIRECTDRAWSURFACE7 bkp_tex, LPDIRECTDRAWSURFACE7 bkp_ltex, DWORD bkp_flag)
{
	ntot++;

	static const double rad0 = sqrt(2.0)*Pi05;
	Vector cnt = TileCentre (hemisp, ilat, nlat, ilng, nlng);
	double rad = rad0/(double)nlat;
	double alpha = acos (dotp (RenderParam.camdir, cnt));
	double adist = alpha - rad;
	if (adist > RenderParam.viewap) return;

	nrad++;

	MATRIX4 W = WorldMatrix (ilng, nlng, ilat, nlat);
	RenderParam.transform = mul (W, RenderParam.viewproj);
	SetWorldMatrix (W);

	// Check if patch bounding box intersects viewport
	if (!TileInView (lvl, ilat)) {
		//tilebuf->DeleteSubTiles (tile); // remove tile descriptions below
		return;
	}

	RenderTile (lvl, hemisp, ilat, nlat, ilng, nlng, tile, range, tex, ltex, flag);
	nrender++;
}

// =======================================================================

MATRIX4 CSphereManager::WorldMatrix (int ilng, int nlng, int ilat, int nlat)
{
	double lng = Pi2 * (double)ilng/(double)nlng + Pi; // add pi so texture wraps at +-180�
	double slng = sin(lng), clng = cos(lng);
	MATRIX4 lrot = {clng,0,slng,0,  0,1.0,0,0,  -slng,0,clng,0,  0,0,0,1.0};
	return mul(lrot,RenderParam.dwmat);
}

// =======================================================================

void CSphereManager::SetWorldMatrix (const MATRIX4 &W)
{
	D3DMATRIX wtrans;
	MATRIX4toD3DMATRIX (W, wtrans);
	RenderParam.dev->SetTransform (D3DTRANSFORMSTATE_WORLD, &wtrans);
}

// =======================================================================

void CSphereManager::RenderTile (int lvl, int hemisp, int ilat, int nlat, int ilng, int nlng,
	TILEDESC *tile, const TEXCRDRANGE &range, LPDIRECTDRAWSURFACE7 tex, LPDIRECTDRAWSURFACE7 ltex, DWORD flag)
{
	LPDIRECT3DVERTEXBUFFER7 vb;        // processed vertex buffer
	VBMESH &mesh = PATCH_TPL[lvl][ilat]; // patch template

	if (range.tumin == 0 && range.tumax == 1) {
		vb = mesh.vb; // use vertex buffer directly
	} else {
		if (!tile->vtx) {
			D3DVERTEXBUFFERDESC vbd = 
				{ sizeof(D3DVERTEXBUFFERDESC), VB_MemFlag | D3DVBCAPS_WRITEONLY, FVF_2TEX, mesh.nv };
			g_pSpaceXpanse->GetInlineGraphicsClient()->GetDirect3D7()->CreateVertexBuffer (&vbd, &tile->vtx, 0);
			ApplyPatchTextureCoordinates (mesh, tile->vtx, range);
			tile->vtx->Optimize (RenderParam.dev, 0); // no more change, so we can optimize
		}
		vb = tile->vtx; // use buffer with transformed texture coords
	}

	RenderParam.dev->SetTexture (0, tex);
	RenderParam.dev->DrawIndexedPrimitiveVB (D3DPT_TRIANGLELIST, vb, 0,
		mesh.nv, mesh.idx, mesh.ni, 0);
}

// =======================================================================
// returns the direction of the tile centre from the planet centre in local
// planet coordinates

Vector CSphereManager::TileCentre (int hemisp, int ilat, int nlat, int ilng, int nlng)
{
	double cntlat = Pi05 * ((double)ilat+0.5)/(double)nlat,      slat = sin(cntlat), clat = cos(cntlat);
	double cntlng = Pi2  * ((double)ilng+0.5)/(double)nlng + Pi, slng = sin(cntlng), clng = cos(cntlng);
	if (hemisp) return Vector (clat*clng, -slat, -clat*slng);
	else        return Vector (clat*clng,  slat,  clat*slng);
}

// =======================================================================

void CSphereManager::TileExtents (int hemisp, int ilat, int nlat, int ilng, int nlng, double &lat1, double &lat2, double &lng1, double &lng2)
{
	lat1 = Pi05 * (double)ilat/(double)nlat;
	lat2 = lat1 + Pi05/(double)nlat;
	lng1 = Pi2 * (double)ilng/(double)nlng + Pi;
	lng2 = lng1 + Pi2/nlng;
	if (hemisp) {
		double tmp = lat1; lat1 = -lat2; lat2 = -tmp;
		tmp = lng1; lng1 = -lng2; lng2 = -tmp;
		if (lng2 < 0) lng1 += Pi2, lng2 += Pi2;
	}
}

// =======================================================================

bool CSphereManager::TileInView (int lvl, int ilat)
{
	bool bx1, bx2, by1, by2, bz1, bbvis;
	int v;
	VBMESH &mesh = PATCH_TPL[lvl][ilat];

	bx1 = bx2 = by1 = by2 = bz1 = bbvis = false;
	double hx, hy, hz;
	for (v = 0; v < 8; v++) {
		VECTOR4 vt = mul (mesh.bbvtx[v], RenderParam.transform);
		hx = vt.x/vt.w, hy = vt.y/vt.w, hz = vt.z/vt.w;
		if (hz <= 1.0) hx = -hx, hy = -hy;
		if (hz >  0.0) bz1 = true;
		if (hx > -1.0) bx1 = true;
		if (hx <  1.0) bx2 = true;
		if (hy > -1.0) by1 = true;
		if (hy <  1.0) by2 = true;
		if (bbvis = bx1 && bx2 && by1 && by2 && bz1) break;
	}

	return bbvis;
}

