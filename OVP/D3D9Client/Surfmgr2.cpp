// ==============================================================
//   SPACEXPANSE VISUALISATION PROJECT (OVP)
//   Copyright (C) 2006-2016 Martin Schweiger
//   Dual licensed under GPL v3 and LGPL v3
// ==============================================================

// ==============================================================
// surfmgr2.cpp
// class SurfaceManager2 (implementation)
//
// Planetary surface rendering engine v2, including a simple
// LOD (level-of-detail) algorithm for surface patch resolution.
// ==============================================================

#include "Surfmgr2.h"
#include "Tilemgr2.h"
#include "Cloudmgr2.h"
#include "D3D9Catalog.h"
#include "D3D9Config.h"
#include "vVessel.h"
#include "VectorHelpers.h"
#include "DebugControls.h"
#include "gcCore.h"
#include "D3D9Surface.h"

// =======================================================================
extern void FilterElevationGraphics(OBJHANDLE hPlanet, int lvl, int ilat, int ilng, float *elev);



// =======================================================================
// Utility functions

static void VtxInterpolate (VERTEX_2TEX &res, const VERTEX_2TEX &a, const VERTEX_2TEX &b, double w)
{
	float w1 = (float)w;
	float w0 = 1.0f-w1;
	res.x = a.x*w0 + b.x*w1;
	res.y = a.y*w0 + b.y*w1;
	res.z = a.z*w0 + b.z*w1;
	res.nx = a.nx*w0 + b.nx*w1;
	res.ny = a.ny*w0 + b.ny*w1;
	res.nz = a.nz*w0 + b.nz*w1;
}


int compare_lights(const void * a, const void * b);


// =======================================================================
// =======================================================================

SurfTile::SurfTile (TileManager2Base *_mgr, int _lvl, int _ilat, int _ilng)
: Tile (_mgr, _lvl, _ilat, _ilng)
{
	smgr = static_cast<TileManager2<SurfTile>* > (_mgr);
	node = 0;
	elev = NULL;
	ggelev = NULL;
	elev_file = NULL;
	ltex = NULL;
	has_elevfile = false;
	label = NULL;
	imicrolvl = 14;	// Water resolution level
	MaxRep = mgr->Client()->GetFramework()->GetCaps()->MaxTextureRepeat;
	if (Config->TileMipmaps == 2) bMipmaps = true;
	if (Config->TileMipmaps == 1 && _lvl < 10) bMipmaps = true;

	memset(&ehdr, 0, sizeof(ELEVFILEHEADER));
}

// -----------------------------------------------------------------------

SurfTile::~SurfTile ()
{
	if (elev) {
		delete []elev;
		elev = NULL;
	}
	if (elev_file) {
		delete []elev_file;
		elev_file = NULL;
	}
	if (ltex && owntex) {
		if (TileCatalog->Remove(ltex)) ltex->Release();
	}
	DeleteLabels();
}

// -----------------------------------------------------------------------

void SurfTile::PreLoad()
{
	char fname[128];
	char path[MAX_PATH];
	bool ok = false;

	// Configure microtexture range for "Water texture" and "Cloud microtexture".
	GetParentMicroTexRange(&microrange);
	GetParentOverlayRange(&overlayrange);

	// Load surface texture

	if (smgr->DoLoadIndividualFiles(0)) { // try loading from individual tile file
		sprintf_s(path, MAX_PATH, "%s\\Surf\\%02d\\%06d\\%06d.dds", mgr->DataRootDir().c_str(), lvl + 4, ilat, ilng);
		ok = LoadTextureFile(path, &pPreSrf);
	}

	if (!ok && smgr->ZTreeManager(0)) { // try loading from compressed archive
		BYTE *buf;
		DWORD ndata = smgr->ZTreeManager(0)->ReadData(lvl+4, ilat, ilng, &buf);
		if (ndata) {
			ok = LoadTextureFromMemory(buf, ndata, &pPreSrf);
			smgr->ZTreeManager(0)->ReleaseData(buf);
		}
	}

	// Load mask texture

	if (ok && (mgr->Cprm().bSpecular || mgr->Cprm().bLights)) {
		ok = false; // <= in case tileLoadFlags is set to "Archive only", we have to reset this, else no (compressed) Mask would be loaded

		if (smgr->DoLoadIndividualFiles(1)) { // try loading from individual tile file
			sprintf_s(path, MAX_PATH, "%s\\Mask\\%02d\\%06d\\%06d.dds", mgr->DataRootDir().c_str(), lvl + 4, ilat, ilng);
			ok = LoadTextureFile(path, &pPreMsk);
		}
		if (!ok && smgr->ZTreeManager(1)) { // try loading from compressed archive
			BYTE *buf;
			DWORD ndata = smgr->ZTreeManager(1)->ReadData(lvl+4, ilat, ilng, &buf);
			if (ndata) {
				LoadTextureFromMemory(buf, ndata, &pPreMsk);
				smgr->ZTreeManager(1)->ReleaseData(buf);
			}
		}
	}

	mgr->TileLabel(pPreSrf, lvl, ilat, ilng);
	mgr->TileLabel(pPreMsk, lvl, ilat, ilng);
}

// -----------------------------------------------------------------------

void SurfTile::Load ()
{

	LPDIRECT3DDEVICE9 pDev = mgr->Dev();

	owntex = true;

	if (CreateTexture(pDev, pPreSrf, &tex) != true) {
		if (GetParentSubTexRange(&texrange)) {
			tex = getSurfParent()->Tex();
			owntex = false;
		}
	} else TileCatalog->Add(tex);

	// Load mask texture
	if (mgr->Cprm().bSpecular || mgr->Cprm().bLights) {
		if (owntex && tex) {
			CreateTexture(pDev, pPreMsk, &ltex);
			if (ltex) TileCatalog->Add(ltex);
		} else if (node && node->Parent()) {
			ltex = getSurfParent()->ltex;
		}
	}

	// Load elevation data
	float *elev = ElevationData ();

	bool shift_origin = (lvl >= 4);
	int res = mgr->GridRes();

	if (lvl <= 0)
	{
		if (!lvl) { // create hemisphere mesh for western or eastern hemispheres
			mesh = CreateMesh_hemisphere(res, elev, 0.0);
	//  } else {    // create full sphere mesh
	//	  // TODO
		}
	} else {
		// create rectangular patch
		mesh = CreateMesh_quadpatch (res, res, elev, 1.0, 0.0, &texrange, shift_origin, &vtxshift, mgr->GetPlanet()->prm.tilebb_excess);
	}

	static const DWORD label_enable = PLN_ENABLE | PLN_LMARK;
	DWORD plnmode = *(DWORD*)smgr->Client()->GetConfigParam(CFGPRM_PLANETARIUMFLAG);
	if ((plnmode & label_enable) == label_enable) {
		CreateLabels();
	}
}

// -----------------------------------------------------------------------

INT16 *SurfTile::ReadElevationFile (const char *name, int lvl, int ilat, int ilng)
{
	const int ndat = TILE_ELEVSTRIDE*TILE_ELEVSTRIDE;
	INT16 *e = NULL;

	// Elevation resolution used for "rounding" due to INT16 elevation. 
	// Technically, should not apply to float based elevation but required due to rounding in physics.
	double tgt_res = mgr->ElevRes();

	char path[MAX_PATH];
	char fname[128];
	FILE *f;
	int i;

	// Elevation data
	if (smgr->DoLoadIndividualFiles(2)) { // try loading from individual tile file
		sprintf_s(path, MAX_PATH, "%s\\Elev\\%02d\\%06d\\%06d.elv", mgr->DataRootDir().c_str(), lvl, ilat, ilng);
		if (!fopen_s(&f, path, "rb")) {
			e = new INT16[ndat];
			elev = new float[ndat];
			// read the elevation file header
			fread (&ehdr, sizeof(ELEVFILEHEADER), 1, f);
			if (ehdr.hdrsize != sizeof(ELEVFILEHEADER)) fseek (f, ehdr.hdrsize, SEEK_SET);
			LogClr("Teal", "NewTile[%s]: Lvl=%d, Scale=%g, Offset=%g", name, lvl-4, ehdr.scale, ehdr.offset);

#ifdef SPACEXPANSE2016
			ehdr.scale = 1.0;
#endif
			switch (ehdr.dtype) {
			case 0: // flat tile, defined by offset
				for (i = 0; i < ndat; i++) e[i] = 0;
				break;
			case 8: {
				UINT8 *tmp = new UINT8[ndat];
				fread (tmp, sizeof(UINT8), ndat, f);
				for (i = 0; i < ndat; i++)
					e[i] = (INT16)tmp[i];
				delete []tmp;
				tmp = NULL;
				}
				break;
			case -16:
				fread (e, sizeof(INT16), ndat, f);
				break;
			}
			fclose (f);
		}
	}
	if (!e && smgr->ZTreeManager(2)) { // try loading from compressed archive
		BYTE *buf;
		DWORD ndata = smgr->ZTreeManager(2)->ReadData(lvl, ilat, ilng, &buf);
		if (ndata) {
			BYTE *p = buf;
			e = new INT16[ndat];
			elev = new float[ndat];
			memcpy(&ehdr, p, sizeof(ELEVFILEHEADER));
			LogClr("Teal", "NewTileA[%s]: Lvl=%d, Scale=%g, Offset=%g", name, lvl-4, ehdr.scale, ehdr.offset);

#ifdef SPACEXPANSE2016
			ehdr.scale = 1.0;
#endif
			p += ehdr.hdrsize;
			switch (ehdr.dtype) {
			case 0:
				for (i = 0; i < ndat; i++) e[i] = 0;
				break;
			case 8:
				for (i = 0; i < ndat; i++)
					e[i] = (INT16)(*p++);
				break;
			case -16:
				memcpy(e, p, ndat*sizeof(INT16));
				p += ndat*sizeof(INT16);
				break;
			}
			smgr->ZTreeManager(2)->ReleaseData(buf);
		}
	}

	if (e) {
	
		//
		//for (i = 0; i < ndat; i++) elev[i] = (trunc(float(e[i]) * (ehdr.scale / tgt_res)) + trunc(ehdr.offset / tgt_res)) * tgt_res;

		if (ehdr.scale != tgt_res) { // rescale the data
			double rescale = ehdr.scale / tgt_res;
			for (i = 0; i < ndat; i++)
				e[i] = (INT16)(e[i] * rescale);
		}
		if (ehdr.offset) {
			INT16 sofs = (INT16)(ehdr.offset / tgt_res);
			for (i = 0; i < ndat; i++)
				e[i] += sofs;
		}

		// Convert to float
		for (i = 0; i < ndat; i++) elev[i] = float(e[i]) * float(tgt_res);	
	}

	// Elevation mod data
	if (e) {
		bool ok = false;
		double rescale;
		INT16 offset;
		bool do_rescale, do_shift;
		ELEVFILEHEADER hdr;
		if (smgr->DoLoadIndividualFiles(3)) { // try loading from individual tile file
			sprintf_s (path, MAX_PATH, "%s\\Elev_mod\\%02d\\%06d\\%06d.elv", mgr->DataRootDir().c_str(), lvl, ilat, ilng);
			if (!fopen_s(&f, path, "rb")) {
				fread (&hdr, sizeof(ELEVFILEHEADER), 1, f);
				if (hdr.hdrsize != sizeof(ELEVFILEHEADER)) fseek (f, hdr.hdrsize, SEEK_SET);
				LogClr("Teal", "NewElevMod[%s]: Lvl=%d, Scale=%g, Offset=%g", name, lvl - 4, hdr.scale, hdr.offset);

#ifdef SPACEXPANSE2016
				hdr.scale = 1.0;
#endif
				rescale = (do_rescale = (hdr.scale != 1.0)) ? hdr.scale : 1.0;
				offset = (do_shift = (hdr.offset != 0.0)) ? INT16(hdr.offset) : 0;

				switch (hdr.dtype) {
				case 0: // overwrite the entire tile with a flat offset
					for (i = 0; i < ndat; i++) e[i] = offset, elev[i] = float(hdr.offset);
					break;
				case 8: {
					const UINT8 mask = UCHAR_MAX;
					UINT8 *tmp = new UINT8[ndat];
					fread (tmp, sizeof(UINT8), ndat, f);
					for (i = 0; i < ndat; i++) {
						if (tmp[i] != mask) {
							e[i] = (INT16)(do_rescale ? (INT16)(tmp[i] * rescale) : (INT16)tmp[i]);
							if (do_shift) e[i] += offset;
							elev[i] = float(e[i]) * float(tgt_res);
						}
					}
					delete []tmp;
					tmp = NULL;
					}
					break;
				case -16: {
					const INT16 mask = SHRT_MAX;
					INT16 *tmp = new INT16[ndat];
					fread (tmp, sizeof(INT16), ndat, f);
					for (i = 0; i < ndat; i++) {
						if (tmp[i] != mask) {
							e[i] = (do_rescale ? (INT16)(tmp[i] * rescale) : tmp[i]);
							if (do_shift) e[i] += offset;
							elev[i] = float(e[i]) * float(tgt_res);
						}
					}
					delete []tmp;
					tmp = NULL;
					}
					break;
				}
				fclose(f);
				ok = true;
			}
		}
		if (!ok && smgr->ZTreeManager(3)) { // try loading from compressed archive
			BYTE *buf;
			DWORD ndata = smgr->ZTreeManager(3)->ReadData(lvl, ilat, ilng, &buf);
			if (ndata) {
				BYTE *p = buf;
				ELEVFILEHEADER *phdr = (ELEVFILEHEADER*)p;
				LogClr("Teal", "NewElevModA[%s]: Lvl=%d, Scale=%g, Offset=%g", name, lvl - 4, phdr->scale, phdr->offset);

#ifdef SPACEXPANSE2016
				phdr->scale = 1.0;
#endif
				p += phdr->hdrsize;
				rescale = (do_rescale = (phdr->scale != 1.0)) ? phdr->scale : 1.0;
				offset = (do_shift = (phdr->offset != 0.0)) ? INT16(phdr->offset) : 0;

				switch(phdr->dtype) {
				case 0:
					for (i = 0; i < ndat; i++) e[i] = offset, elev[i] = float(phdr->offset);
					break;
				case 8: {
					const UINT8 mask = UCHAR_MAX;
					for (i = 0; i < ndat; i++) {
						if (p[i] != mask) {
							e[i] = (INT16)(do_rescale ? p[i] * rescale : p[i]);
							if (do_shift) e[i] += offset;
							elev[i] = float(e[i]) * float(tgt_res);
						}
					}
					} break;
				case -16: {
					const INT16 mask = SHRT_MAX;
					INT16 *buf16 = (INT16*)p;
					for (i = 0; i < ndat; i++) {
						if (buf16[i] != mask) {
							e[i] = (do_rescale ? (INT16)(buf16[i] * rescale) : buf16[i]);
							if (do_shift) e[i] += offset;
							elev[i] = float(e[i]) * float(tgt_res);
						}
					}
					} break;
				}
				smgr->ZTreeManager(3)->ReleaseData(buf);
			}
		}
		if (Config->bFlats) FilterElevationGraphics(mgr->GetPlanet()->Object(), lvl - 4, ilat, ilng, elev);
	}
	return e;
}

// -----------------------------------------------------------------------

LPDIRECT3DTEXTURE9 SurfTile::SetOverlay(LPDIRECT3DTEXTURE9 pOverlay, bool bOwn)
{
	LPDIRECT3DTEXTURE9 pRet = NULL;
	if (bOwn && ownoverlay && overlay) pRet = overlay;

	overlay = pOverlay;
	ownoverlay = bOwn;

	for (int i = 0; i < 4; i++) {
		auto x = node->Child(i);
		if (x) if (x->Entry()) {
			LPDIRECT3DTEXTURE9 pOld = x->Entry()->overlay;
			if (pOld == NULL || pOld == pRet) {
				x->Entry()->GetParentOverlayRange(&overlayrange);
				x->Entry()->SetOverlay(pOverlay, false);
			}
		}
	}
	return pRet;
}

// -----------------------------------------------------------------------

bool SurfTile::DeleteOverlay(LPDIRECT3DTEXTURE9 pOverlay)
{
	bool bReturn = false;
	if (pOverlay == NULL) pOverlay = overlay;

	if (overlay) {
		if (pOverlay == overlay) {
			if (ownoverlay) {
				if (TileCatalog->Remove(overlay)) overlay->Release();
				bReturn = true;
			}
			for (int i = 0; i < 4; i++) {
				auto x = node->Child(i);
				if (x) if (x->Entry()) x->Entry()->DeleteOverlay(pOverlay);
			}
			overlay = NULL;
			ownoverlay = false;
		}
	}
	return bReturn;
}


// -----------------------------------------------------------------------

float SurfTile::Interpolate(FMATRIX4 &in, float t, float u)
{
	return 0.0f;
}

// -----------------------------------------------------------------------

bool SurfTile::InterpolateElevationGrid(const float *in, float *out)
{
	int q0 = 0, c = 129;

	if (!(ilat & 1)) q0 = TILE_ELEVSTRIDE * 128;
	if ((ilng & 1)) q0 += 128;

	for (int i = 0; i <= c; i++)
	{
		int q1 = q0 + 1;
		int q2 = q0 + TILE_ELEVSTRIDE;
		int q3 = q1 + TILE_ELEVSTRIDE;
		int x = (TILE_ELEVSTRIDE << 1) * i;

		for (int k = 0; k <= c; k++)
		{
			float f0 = in[k + q0];	float f1 = in[k + q1];
			float f2 = in[k + q2];	float f3 = in[k + q3];

			out[x + 0] = (f0 + f1 + f2 + f3) * 0.25f;
			if (k != c) out[x + 1] = (f1 + f3) * 0.5f;

			if (i != c) {
				out[x + TILE_ELEVSTRIDE] = (f2 + f3) * 0.5f;
				if (k != c) out[x + TILE_ELEVSTRIDE + 1] = f3;
			}
			x += 2;
		}
		q0 += TILE_ELEVSTRIDE;
	}
	return true;
}


// -----------------------------------------------------------------------

bool SurfTile::LoadElevationData ()
{
	// Note: a tile's elevation data are retrieved from its great-grandparent tile. Each tile stores the elevation
	// data for its area at 8x higher resolution than required by itself, so they can be used by the grandchildren.
	// This reduces the number of elevation files, by storing more information in each. It also has a caching effect:
	// Once a grandparent has loaded its elevation data on request of a grandchild, any other grandchildren's requests
	// can be served directly without further disk I/O.

	if (elev) return true; // already present

	has_elevfile = false;
	int mode = mgr->Cprm().elevMode;
	if (!mode) return false;

	DWORD phy_lvl = mgr->GetPlanet()->GetPhysicsPatchRes();
	int ndat = TILE_ELEVSTRIDE*TILE_ELEVSTRIDE;

	elev_file = ReadElevationFile (mgr->CbodyName(), lvl + 4, ilat, ilng);
	double tgt_res = mgr->ElevRes();

	if (elev_file) has_elevfile = true;
	else if (lvl > 0) {

		// Acquire elev header data from a parent
		QuadTreeNode<SurfTile> *parent = node->Parent();
		if (parent &&  parent->Entry()) memcpy(&ehdr, &parent->Entry()->ehdr, sizeof(ELEVFILEHEADER));

		// construct elevation grid by interpolating ancestor data
		ELEVHANDLE hElev = mgr->ElevMgr();
		if (!hElev) return false;

		// Cubic Interpolation
		if (mode == 2) {

			int plvl = lvl-1;
			int pilat = ilat >> 1;
			int pilng = ilng >> 1;
			INT16 *pelev_file = 0;
			QuadTreeNode<SurfTile> *parent = node->Parent();
			for (; plvl >= 0; plvl--) { // find ancestor with elevation data
				if (parent && parent->Entry()->has_elevfile) {
					pelev_file = parent->Entry()->elev_file;
					break;
				}
				if (parent) parent = parent->Parent();
				pilat >>= 1;
				pilng >>= 1;
			}

			if (!pelev_file) return false;

			elev = new float[ndat];

			// submit ancestor data to elevation manager for interpolation
			mgr->GetClient()->ElevationGrid(hElev, ilat, ilng, lvl, pilat, pilng, plvl, pelev_file, elev);

			// Convert to float
			for (int i = 0; i < ndat; i++) elev[i] *= float(tgt_res);
		}

		// Experimental Linear Interpolation
		else {
			QuadTreeNode<SurfTile> *parent = node->Parent();
			if (parent &&  parent->Entry()->elev) {
				elev = new float[ndat];
				InterpolateElevationGrid(parent->Entry()->elev, elev);
			}
		}
	}

	if (has_elevfile) LogClr("Teal", "TileCreatedFromFile: Level=%d, ilat=%d, ilng=%d", lvl, ilat, ilng);
	else LogClr("Teal", "TileInterpolatedFromParent: Level=%d, ilat=%d, ilng=%d", lvl, ilat, ilng);

	return (elev != 0);
}

// -----------------------------------------------------------------------

float *SurfTile::ElevationData () const
{
	if (!ggelev) {
		int ancestor_dlvl = 3;
		while (1 << (8-ancestor_dlvl) < mgr->GridRes()) ancestor_dlvl--;
		if (lvl >= ancestor_dlvl) { // traverse quadtree back to great-grandparent
			QuadTreeNode<SurfTile> *ancestor = node; // start at my own node
			int blockRes = TILE_FILERES;
			while (blockRes > mgr->GridRes() && ancestor) {
				ancestor = ancestor->Parent();
				blockRes >>= 1;
			}
			if (ancestor && ancestor->Entry()->LoadElevationData()) {
				// compute pixel offset into great-grandparent tile set
				int nblock = TILE_FILERES/blockRes;
				int mask = nblock-1;
				int ofs = ((mask - ilat & mask) * TILE_ELEVSTRIDE + (ilng & mask)) * blockRes;
				ggelev = ancestor->Entry()->elev + ofs;
			}
		} else {
			SurfTile *ggp = smgr->GlobalTile(lvl - ancestor_dlvl);// +3);
			if (ggp && ggp->LoadElevationData ()) {
				int blockRes = mgr->GridRes();
				int nblock = TILE_FILERES/blockRes;
				int mask = nblock-1;
				int ofs = ((mask - ilat & mask) * TILE_ELEVSTRIDE + (ilng & mask)) * blockRes;
				ggelev = ggp->elev + ofs;
			}
		}
		if (ggelev) ComputeElevationData(ggelev);
	}
	return ggelev;
}

// -----------------------------------------------------------------------

void SurfTile::ComputeElevationData(const float *elev) const
{
	if (has_elevfile) return;
	int i, j;
	int res = mgr->GridRes();
	ehdr.emax = -1e30;
	ehdr.emin = 1e30;
	ehdr.emean = 0.0;
	for (j = 0; j <= res; j++) {
		for (i = 0; i <= res; i++) {
			ehdr.emean += elev[i];
			ehdr.emax = max(ehdr.emax, elev[i]);
			ehdr.emin = min(ehdr.emin, elev[i]);
		}
		elev += TILE_ELEVSTRIDE;
	}
	ehdr.emean /= double((res + 1)*(res + 1));
}

// ------------------------------------------------------------------------------
// bGet(true) = Get the data specifically from this tile recardless of it's state
//
int SurfTile::GetElevation(double lng, double lat, double *elev, FVECTOR3 *nrm, SurfTile **cache, bool bFilter, bool bGet) const
{
	static int ndat = TILE_ELEVSTRIDE*TILE_ELEVSTRIDE;
	if (cache) *cache = (SurfTile *)this;

	if (lat<bnd.minlat || lat>bnd.maxlat) return -1;
	if (lng<bnd.minlng || lng>bnd.maxlng) return -1;

	if (state == ForRender || bGet)
	{
		if (!ggelev) { *elev = 0.0; return 2; }
		else {
			double fRes = double(mgr->GridRes());
			
			if (!bFilter) {
				int i = int((lat - bnd.minlat) * fRes / (bnd.maxlat - bnd.minlat)) + 1;
				int j = int((lng - bnd.minlng) * fRes / (bnd.maxlng - bnd.minlng)) + 1;
				*elev = double(ggelev[j+i*TILE_ELEVSTRIDE]);
			}
			else {

				float x = float((lat - bnd.minlat) * fRes / (bnd.maxlat - bnd.minlat)) + 1.0f; // 0.5f
				float y = float((lng - bnd.minlng) * fRes / (bnd.maxlng - bnd.minlng)) + 1.0f; // 0.5f
				float fx = (x - floor(x));
				float fy = (y - floor(y));

				int i0 = int(x) * TILE_ELEVSTRIDE;
				int i1 = i0 + TILE_ELEVSTRIDE;
				int j0 = int(y);

				assert(i0 > 0);
				assert(j0 > 0);
				assert((j0 + i1) < ndat);

				float q = lerp(ggelev[j0+i0], ggelev[j0+i1], fx); j0++;
				float w = lerp(ggelev[j0+i0], ggelev[j0+i1], fx);

				*elev = double(lerp(q,w,fy));
			}

			return 1;
		}
	}

	if (state == Invisible) return 0;

	if (state==Active) {
		int i = 0;
		if (lng > (bnd.minlng + bnd.maxlng)*0.5) i++;
		if (lat < (bnd.minlat + bnd.maxlat)*0.5) i += 2;
		return  node->Child(i)->Entry()->GetElevation(lng, lat, elev, nrm, cache);
	}

	return -3;
}

// -----------------------------------------------------------------------

SurfTile *SurfTile::getTextureOwner()
{
	if (owntex) return this;
	SurfTile *parent = getSurfParent();
	while (parent) {
		if (parent->owntex) return parent;
		else parent = parent->getSurfParent();
	}
	return NULL;
}

// -----------------------------------------------------------------------

float SurfTile::fixinput(double a, int x)
{
	switch(x) {
		case 0: return float((1.0+floor(a*0.0625))*16.0);
		case 1: return float((1.0+floor(a*0.125))*8.0);
		case 2: return float((1.0+floor(a*0.5))*2.0);
	}
	return 0.0f;
}

// -----------------------------------------------------------------------

double SurfTile::GetCameraDistance()
{
	VECTOR3 cnt = Centre() * (mgr->CbodySize() + GetMeanElev());
	cnt = mgr->prm.cpos + mul(mgr->prm.grot, cnt);
	return length(cnt);
}

// -----------------------------------------------------------------------

D3DXVECTOR4 SurfTile::MicroTexRange(SurfTile *pT, int ml) const
{
	float rs = 1.0f / float( 1 << (lvl-pT->Level()) );	// Range subdivision
	float xo = pT->MicroRep[ml].x * texrange.tumin;
	float yo = pT->MicroRep[ml].y * texrange.tvmin;
	xo -= floor(xo); // Micro texture offset for current tile
	yo -= floor(yo); // Micro texture offset for current tile
	return D3DXVECTOR4(xo, yo, pT->MicroRep[ml].x * rs, pT->MicroRep[ml].y * rs);
}

// -----------------------------------------------------------------------
// Called during rendering even if this tile is not rendered but children are
// i.e. called for every RENDERED and ACTIVE tile
//
void SurfTile::StepIn ()
{
	LPDIRECT3DDEVICE9 pDev = mgr->Dev();
	ID3DXEffect *Shader = mgr->Shader();
	const vPlanet *vPlanet = mgr->GetPlanet();

	if (vPlanet != mgr->GetScene()->GetCameraProxyVisual()) return;

	// Compute micro texture repeat counts -------------------------------------
	//
	if (owntex && vPlanet->MicroCfg.bEnabled) {
		double s = vPlanet->GetSize();
		for (int i = 0; i < ARRAYSIZE(vPlanet->MicroCfg.Level); ++i) {
			double f = s / vPlanet->MicroCfg.Level[i].size;
			MicroRep[i] = D3DXVECTOR2(fixinput(width*f, i), fixinput(height*f, i));
		}
	}
}


// -----------------------------------------------------------------------

void SurfTile::Render ()
{
	bool render_lights = mgr->Cprm().bLights;
	bool render_shadows = mgr->GetPlanet()->CloudMgr2()!=NULL; // && !mgr->prm.rprm->bCloudFlatShadows);

	if (!mesh) return; // DEBUG : TEMPORARY

	UINT numPasses = 0;

	LPDIRECT3DDEVICE9 pDev = mgr->Dev();
	ID3DXEffect *Shader = mgr->Shader();
	const vPlanet *vPlanet = mgr->GetPlanet();
	const Scene *scene = mgr->GetScene();
	const D3D9Client *pClient = mgr->GetClient();

	if (scene->GetRenderPass() == RENDERPASS_MAINSCENE) mgr->SetMinMaxElev(ehdr.emin, ehdr.emax);

	static const double rad0 = sqrt(2.0)*PI05;
	double sdist, rad;
	bool has_specular = false;
	bool has_shadows = false;
	bool has_lights = false;
	bool has_microtex = false;
	bool has_atmosphere = vPlanet->HasAtmosphere();
	bool has_ripples = vPlanet->HasRipples();


	if (ltex || render_shadows) {
		sdist = acos (dotp (mgr->prm.sdir, cnt));
		rad = rad0/(double)(2<<lvl); // tile radius
		has_specular = (ltex && sdist < (1.75 + rad));
		has_shadows = (render_shadows && sdist < (PI05+rad));
		has_lights = (render_lights && ltex && sdist > 1.35);
	}

	if (vPlanet->CameraAltitude()>20e3) has_ripples = false;

	double ca = 1.0 + saturate(vPlanet->CameraAltitude() / 150e3) * Config->OrbitalShadowMult;

	HR(Shader->SetVector(TileManager2Base::svCloudOff, &D3DXVECTOR4(0, 0, 1, 1)));


	// Assign micro texture range information to shaders -------------------------
	//
	SurfTile *pT = getTextureOwner();

	if (pT && vPlanet->MicroCfg.bEnabled) {
		HR(Shader->SetVector(TileManager2Base::svMicroScale0, &MicroTexRange(pT, 0)));
		HR(Shader->SetVector(TileManager2Base::svMicroScale1, &MicroTexRange(pT, 1)));
		HR(Shader->SetVector(TileManager2Base::svMicroScale2, &MicroTexRange(pT, 2)));
		has_microtex = true;
	}



	// Setup cloud shadows -------------------------------------------------------
	//
	if (has_shadows) {

		has_shadows = false;

		const TileManager2<CloudTile> *cmgr = vPlanet->CloudMgr2();
		int maxlvl = min(lvl,9);
		double rot = mgr->prm.rprm->cloudrot;
		double edglat = (bnd.minlat + bnd.maxlat)*0.5;	// latitude of tile center
		double edglng = wrap(rot + bnd.minlng);		// surface tile minlng-edge position on cloud-layer

		for (int attempt=0;attempt<2;attempt++) {

			CloudTile *ctile = (CloudTile *)cmgr->SearchTile(edglng, edglat, maxlvl, true);

			if (ctile) {

				double icsize = double( 1 << ctile->Level() ) / PI;	// inverse of cloud tile size in radians

				// Compute surface tile uv origin on a selected claud tile
				// Note: edglng exists always within tile i.e. no wrap from PI to -PI
				double u0 = (edglng - ctile->bnd.minlng) * icsize;
				double v0 = (ctile->bnd.maxlat - bnd.maxlat) * icsize;		// Note: Tile corner is lower-left, texture corner is upper-left
				double u1 = u0 + (bnd.maxlng - bnd.minlng) * icsize;
				double v1 = v0 + (bnd.maxlat - bnd.minlat) * icsize;

				// Feed uv-offset and uv-range to the shaders
				HR(Shader->SetVector(TileManager2Base::svCloudOff, &D3DXVECTOR4(float(u0), float(v0), float(u1-u0), float(v1-v0))));
				HR(Shader->SetFloat(TileManager2Base::sfAlpha, float(ca)));
				HR(Shader->SetTexture(TileManager2Base::stCloud, ctile->Tex()));

				// Texture uv range extends to another tile
				if (u1 > 1.0) {

					double csize = PI / double(1<<ctile->Level());			// cloud tile size in radians
					double ctr = (ctile->bnd.minlng + ctile->bnd.maxlng) * 0.5;		// cloud tile center
					double lng  = wrap(ctr + csize*sign(rot));				// center of the next tile

					// Request an other tile from the same level as the first one
					CloudTile *ctile2 = (CloudTile *)cmgr->SearchTile(lng, edglat, ctile->Level(), true);

					if (ctile2) {
						if (ctile2->Level() == ctile->Level()) {
							// Rendering with dual texture
							HR(Shader->SetTexture(TileManager2Base::stCloud2, ctile2->Tex()));
							has_shadows = true;
							break;
						}
						else {
							// Failed to match a dual texture render requirements (due to no-valid tile available)
							// Try again and request a lower level texture data for the first texture
							maxlvl = ctile2->Level();
							if (attempt==1) LogErr("CloudShadows mapping failed");
						}
					}
				}
				else {
					// Just one texture in use
					has_shadows = true;
					break;
				}
			}
		}
	}


	// ---------------------------------------------------------------------------------------------------
	// Render with overlay image
	//
	D3DXVECTOR4 texcoord;
	const vPlanet::sOverlay *oLay = vPlanet->IntersectOverlay(bnd.vec, &texcoord);

	if (oLay)
	{
		bool bOlayEnable = false; for (auto x : oLay->pSurf) if (x) bOlayEnable = true;

		if (bOlayEnable)
		{
			// Global large-scale overlay
			HR(Shader->SetTexture(TileManager2Base::stOverlay, oLay->pSurf[0]));
			HR(Shader->SetTexture(TileManager2Base::stMskOverlay, oLay->pSurf[1]));
			HR(Shader->SetTexture(TileManager2Base::stElvOverlay, oLay->pSurf[2]));
			HR(Shader->SetVectorArray(TileManager2Base::svOverlayCtrl, oLay->Blend, 4));
			HR(Shader->SetVector(TileManager2Base::svOverlayOff, &texcoord));

			if (oLay->pSurf[0] || oLay->pSurf[1]) {
				HR(Shader->SetBool(TileManager2Base::sbOverlay, true));
			}
			if (oLay->pSurf[2]) {
				HR(Shader->SetBool(TileManager2Base::sbElevOvrl, true));
			}
		}
		else {
			// No Overlay
			HR(Shader->SetTexture(TileManager2Base::stOverlay, NULL));
			HR(Shader->SetTexture(TileManager2Base::stMskOverlay, NULL));
			HR(Shader->SetTexture(TileManager2Base::stElvOverlay, NULL));
			HR(Shader->SetBool(TileManager2Base::sbOverlay, false));
			HR(Shader->SetBool(TileManager2Base::sbElevOvrl, false));
		}
	} else if (overlay) {
		// Local tile specific overlay
		HR(Shader->SetTexture(TileManager2Base::stOverlay, overlay));
		HR(Shader->SetVector(TileManager2Base::svOverlayOff, &GetTexRangeDX(&overlayrange)));
		HR(Shader->SetBool(TileManager2Base::sbOverlay, true));
	} else {
		// No Overlay
		HR(Shader->SetTexture(TileManager2Base::stOverlay, NULL));
		HR(Shader->SetBool(TileManager2Base::sbOverlay, false));
	}


	// ---------------------------------------------------------------------------------------------------
	// Feed tile specific data to shaders
	//
	// ---------------------------------------------------------------------------------------------------
	HR(Shader->SetTexture(TileManager2Base::stDiff, tex));
	HR(Shader->SetTexture(TileManager2Base::stMask, ltex));
	HR(Shader->SetVector(TileManager2Base::svTexOff, &GetTexRangeDX(&texrange)));
	HR(Shader->SetVector(TileManager2Base::svMicroOff, &GetTexRangeDX(&microrange)));
	// ---------------------------------------------------------------------------------------------------
	HR(Shader->SetBool(TileManager2Base::sbCloudSh, has_shadows));
	HR(Shader->SetBool(TileManager2Base::sbLights, has_lights));
	// ---------------------------------------------------------------------------------------------------
	if (has_lights) { HR(Shader->SetFloat(TileManager2Base::sfNight, float(mgr->Cprm().lightfac))); }
	else {			  HR(Shader->SetFloat(TileManager2Base::sfNight, 0.0f));	}




	// ---------------------------------------------------------------------
	// Setup shadow maps
	// ---------------------------------------------------------------------

	D3DXMATRIX wmx;
	HR(Shader->GetMatrix(TileManager2Base::smWorld, &wmx));
	HR(Shader->SetBool(TileManager2Base::sbShadows, false));

	D3DXVECTOR3 bs_pos;
	D3DXVec3TransformCoord(&bs_pos, &mesh->bsCnt, &wmx);

	const Scene::SHADOWMAPPARAM *shd = scene->GetSMapData();

	D3DXVECTOR3 bc = bs_pos - shd->pos;

	double alt = scene->GetCameraAltitude() - scene->GetTargetElevation();

	if ((alt < 10e3) && (scene->GetCameraProxyVisual() == mgr->GetPlanet())) {

		if (shd->pShadowMap && (Config->ShadowMapMode != 0) && (Config->TerrainShadowing == 2)) {

			float x = D3DXVec3Dot(&bc, &(shd->ld));

			if (sqrt(D3DXVec3Dot(&bc, &bc) - x*x) < (shd->rad + mesh->bsRad)) {
				float s = float(shd->size);
				float sr = 2.0f * shd->rad / s;
				HR(Shader->SetMatrix(TileManager2Base::smLVP, &shd->mViewProj));
				HR(Shader->SetVector(TileManager2Base::svSHD, &D3DXVECTOR4(sr, 1.0f / s, 0, 1.0f / shd->depth)));
				HR(Shader->SetTexture(TileManager2Base::stShadowMap, shd->pShadowMap));
				HR(Shader->SetBool(TileManager2Base::sbShadows, true));
			}
		}
	}




	// ---------------------------------------------------------------------
	// Setup local light sources
	//---------------------------------------------------------------------

	const D3D9Light *pLights = scene->GetLights();
	int nSceneLights = scene->GetLightCount();

	LightStruct Locals[4];

	HR(Shader->SetBool(TileManager2Base::sbLocals, false));


	if (pLights && nSceneLights>0) {

		int nMeshLights = 0;

		_LightList LightList[MAX_SCENE_LIGHTS];

		// Find all local lights effecting this mesh ------------------------------------------
		//
		for (int i = 0; i < nSceneLights; i++) {
			float il = pLights[i].GetIlluminance(bs_pos, mesh->bsRad);
			if (il > 0.005f) {
				LightList[nMeshLights].illuminace = il;
				LightList[nMeshLights++].idx = i;
			}
		}

		if (nMeshLights > 0) {

			// If any, Sort the list based on illuminance -------------------------------------------
			qsort(LightList, nMeshLights, sizeof(_LightList), compare_lights);

			nMeshLights = min(nMeshLights, 4);

			// Create a list of N most effective lights ---------------------------------------------
			for (int i = 0; i < nMeshLights; i++) memcpy(&Locals[i], &pLights[LightList[i].idx], sizeof(LightStruct));

			HR(Shader->SetBool(TileManager2Base::sbLocals, true));
			HR(Shader->SetValue(TileManager2Base::ssLight, &Locals, sizeof(Locals)));
		}
	}

	/*
	if (pLights && nSceneLights > 0) {

		// Create a list of N most effective lights ---------------------------------------------
		for (int i = 0; i < nSceneLights; i++) memcpy(&Locals[i], &pLights[i], sizeof(LightStruct));
		HR(Shader->SetBool(TileManager2Base::sbLocals, true));

	}*/

	Shader->CommitChanges();

	// -------------------------------------------------------------------
	// Find suitable technique

	int iTech = 0;

	if (has_atmosphere) {
		if (has_shadows) {
			if (has_ripples) iTech = 0;		// Earth
			else			 iTech = 1;		// Earth, no ripples
		}
		else {
			if (has_microtex) iTech = 2;	// Mars
			else			  iTech = 3;	// Mars, no micro
		}
	}
	else {
		if (has_microtex) iTech = 4;	// Luna
		else			  iTech = 5;	// Luna, no micro
	}


	// -------------------------------------------------------------------
	// render surface mesh
	//
	HR(Shader->Begin(&numPasses, D3DXFX_DONOTSAVESTATE));
	HR(Shader->BeginPass(iTech));
	pDev->SetVertexDeclaration(pPatchVertexDecl);
	pDev->SetStreamSource(0, mesh->pVB, 0, sizeof(VERTEX_2TEX));
	pDev->SetIndices(mesh->pIB);
	pDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, mesh->nv, 0, mesh->nf);
	HR(Shader->EndPass());
	HR(Shader->End());

	// Render tile bounding box
	//
	if (DebugControls::IsActive()) {
		DWORD flags  = *(DWORD*)mgr->GetClient()->GetConfigParam(CFGPRM_GETDEBUGFLAGS);
		if (flags&DBG_FLAGS_TILEBOXES) {
			D3D9Effect::RenderTileBoundingBox(&mWorld, mesh->Box, &D3DXVECTOR4(1,0,0,1));
		}
	}
}

// -----------------------------------------------------------------------

void SurfTile::MatchEdges ()
{
	if (edgeok) return; // done already
	edgeok = true;
	if (!mesh) return;  // sanity check

	QuadTreeNode<SurfTile>* lngnbr = smgr->FindNode(lvl, ilng + (ilng & 1 ? 1 : -1), ilat);
	QuadTreeNode<SurfTile>* latnbr = smgr->FindNode(lvl, ilng, ilat + (ilat & 1 ? 1 : -1));
	QuadTreeNode<SurfTile>* dianbr = smgr->FindNode(lvl, ilng + (ilng & 1 ? 1 : -1), ilat + (ilat & 1 ? 1 : -1));


	if (lngnbr && !(lngnbr->Entry()->state & TILE_VALID)) lngnbr = 0;
	if (latnbr && !(latnbr->Entry()->state & TILE_VALID)) latnbr = 0;
	if (dianbr && !(dianbr->Entry()->state & TILE_VALID)) dianbr = 0;

	int new_lngnbr_lvl = (lngnbr ? lngnbr->Entry()->lvl : lvl);
	int new_latnbr_lvl = (latnbr ? latnbr->Entry()->lvl : lvl);
	int new_dianbr_lvl = (dianbr ? dianbr->Entry()->lvl : lvl);

	// fix lower-level neighbours first
	bool nbr_updated = false;
	if (new_lngnbr_lvl < lvl) {
		lngnbr->Entry()->MatchEdges ();
		nbr_updated = true;
	}
	if (new_latnbr_lvl < lvl) {
		latnbr->Entry()->MatchEdges ();
		nbr_updated = true;
	}
	if (!nbr_updated && new_dianbr_lvl < lvl) {
		// now we need to check the diagonal neighbour, since this could be lower resolution
		// and thus responsible for the corner node elevation
		dianbr->Entry()->MatchEdges ();
	}

	bool lngedge_changed = (new_lngnbr_lvl != lngnbr_lvl);
	bool latedge_changed = (new_latnbr_lvl != latnbr_lvl);
	bool diaedge_changed = (new_dianbr_lvl != dianbr_lvl);

	if (lngedge_changed || latedge_changed || diaedge_changed) {
		if (new_dianbr_lvl < new_lngnbr_lvl && new_dianbr_lvl < new_latnbr_lvl) {
			FixCorner (dianbr ? dianbr->Entry() : 0);
			FixLatitudeBoundary (latnbr ? latnbr->Entry() : 0, true);
			FixLongitudeBoundary (lngnbr ? lngnbr->Entry() : 0, true);
		} else if (new_latnbr_lvl < new_lngnbr_lvl) {
			FixLatitudeBoundary (latnbr ? latnbr->Entry() : 0);
			FixLongitudeBoundary (lngnbr ? lngnbr->Entry() : 0, true);
		} else {
			FixLongitudeBoundary (lngnbr ? lngnbr->Entry() : 0);
			FixLatitudeBoundary (latnbr ? latnbr->Entry() : 0, true);
		}
		mesh->MapVertices(mgr->Dev()); // copy the updated vertices to the vertex buffer
		lngnbr_lvl = new_lngnbr_lvl;
		latnbr_lvl = new_latnbr_lvl;
		dianbr_lvl = new_dianbr_lvl;
	}
}

// -----------------------------------------------------------------------

void SurfTile::FixCorner (const SurfTile *nbr)
{
	if (!mesh) return; // sanity check

	int res = mgr->GridRes();
	int vtx_idx = (ilat & 1 ? 0 : (res+1)*res) + (ilng & 1 ? res : 0);
	VERTEX_2TEX &vtx_store = mesh->vtx[mesh->nv + (ilat & 1 ? 0 : res)];

	float *elev = ElevationData();
	if (!elev) return;

	if (nbr) {
		float *nbr_elev = nbr->ElevationData();
		if (!nbr_elev) return;

		float corner_elev = elev[TILE_ELEVSTRIDE+1 + (ilat & 1 ? 0 : TILE_ELEVSTRIDE*res) + (ilng & 1 ? res : 0)];
		float nbr_corner_elev = nbr_elev[TILE_ELEVSTRIDE+1 + (ilat & 1 ? TILE_ELEVSTRIDE*res : 0) + (ilng & 1 ? 0 : res)];

		double rad = mgr->CbodySize();
		double radfac = (rad+nbr_corner_elev)/(rad+corner_elev);
		mesh->vtx[vtx_idx].x = (float)(vtx_store.x*radfac + vtxshift.x*(radfac-1.0));
		mesh->vtx[vtx_idx].y = (float)(vtx_store.y*radfac + vtxshift.y*(radfac-1.0));
		mesh->vtx[vtx_idx].z = (float)(vtx_store.z*radfac + vtxshift.z*(radfac-1.0));
	}
}

// -----------------------------------------------------------------------

void SurfTile::FixLongitudeBoundary (const SurfTile *nbr, bool keep_corner)
{
	// Fix the left or right edge
	int i, i0, i1, j, vtx_ofs, nbrlvl, dlvl;
	int res = mgr->GridRes();
	VERTEX_2TEX *vtx_store;

	vtx_ofs = (ilng & 1 ? res : 0); // check if neighbour is at left or right edge
	if (nbr && nbr->mesh && nbr->mesh->vtx) {
		nbrlvl = min(nbr->lvl, lvl); // we don't need to worry about neigbour levels higher than ours
		vtx_store = mesh->vtx + mesh->nv;
		if (nbrlvl == lvl) { // put my own edge back
			i0 = 0;
			i1 = res;
			if (keep_corner)
				if (ilat & 1) i0++; else i1--;
			for (i = i0; i <= i1; i++)
				mesh->vtx[vtx_ofs+i*(res+1)] = vtx_store[i];
		} else {  // interpolate to neighbour's left edge
			dlvl = lvl-nbrlvl;
			int nsub = 1 << dlvl; // number of tiles fitting alongside the lowres neighbour
			if (nsub <= res) { // for larger tile level differences the interleaved sampling method doesn't work
				float *elev = ElevationData();
				float *nbr_elev = nbr->ElevationData();
				if (elev && nbr_elev) {
					elev += TILE_ELEVSTRIDE+1 + vtx_ofs; // add offset
					int nsub = 1 << dlvl;    // number of tiles fitting alongside the lowres neighbour
					int vtx_skip = nsub*(res+1);  // interleave factor for nodes attaching to neigbour nodes
					int nbr_range = res/nsub; // number of neighbour vertices attaching to us - 1
					int subidxmask = (1<<dlvl)-1;
					int subidx = ilat & subidxmask;
					int nbr_ofs = (subidxmask-subidx) * nbr_range * TILE_ELEVSTRIDE;
					nbr_elev += TILE_ELEVSTRIDE+1 + nbr_ofs + (res-vtx_ofs);
					double rad = mgr->CbodySize();
					// match nodes to neighbour elevations
					i0 = 0;
					i1 = nbr_range;
					if (keep_corner)
						if (ilat & 1) i0++; else i1--;
					for (i = i0; i <= i1; i++) {
						double radfac = (rad+nbr_elev[i*TILE_ELEVSTRIDE])/(rad+elev[i*TILE_ELEVSTRIDE*nsub]);
						mesh->vtx[vtx_ofs+i*vtx_skip].x = (float)(vtx_store[i*nsub].x*radfac + vtxshift.x*(radfac-1.0));
						mesh->vtx[vtx_ofs+i*vtx_skip].y = (float)(vtx_store[i*nsub].y*radfac + vtxshift.y*(radfac-1.0));
						mesh->vtx[vtx_ofs+i*vtx_skip].z = (float)(vtx_store[i*nsub].z*radfac + vtxshift.z*(radfac-1.0));
					}
					// interpolate the nodes that fall between neighbour nodes
					for (i = 0; i < nbr_range; i++)
						for (j = 1; j < nsub; j++)
							VtxInterpolate (mesh->vtx[vtx_ofs+j*(res+1)+i*vtx_skip], mesh->vtx[vtx_ofs+i*vtx_skip], mesh->vtx[vtx_ofs+(i+1)*vtx_skip], (double)j/double(nsub));
				}
			} else {
				// problems
			}
		}
	}
}

// -----------------------------------------------------------------------

void SurfTile::FixLatitudeBoundary (const SurfTile *nbr, bool keep_corner)
{
	// Fix the top or bottom edge
	int i, i0, i1, j, nbrlvl, dlvl;
	int res = mgr->GridRes();
	VERTEX_2TEX *vtx_store;

	int line = (ilat & 1 ? 0 : res);
	int vtx_ofs = (ilat & 1 ? 0 : line*(res+1));
	if (nbr && nbr->mesh && nbr->mesh->vtx) {
		nbrlvl = min(nbr->lvl, lvl); // we don't need to worry about neigbour levels higher than ours
		vtx_store = mesh->vtx + mesh->nv + res + 1;
		if (nbrlvl == lvl) { // put my own edge back
			i0 = 0;
			i1 = res;
			if (keep_corner)
				if (ilng & 1) i1--; else i0++;
			for (i = i0; i <= i1; i++)
				mesh->vtx[vtx_ofs+i] = vtx_store[i];
		} else {
			dlvl = lvl-nbrlvl;
			int nsub = 1 << dlvl; // number of tiles fitting alongside the lowres neighbour
			if (nsub <= res) { // for larger tile level differences the interleaved sampling method doesn't work
				float *elev = ElevationData();
				float *nbr_elev = nbr->ElevationData();
				if (elev && nbr_elev) {
					elev += (line+1)*TILE_ELEVSTRIDE + 1; // add offset
					int nsub = 1 << dlvl;    // number of tiles fitting alongside the lowres neighbour
					int vtx_skip = nsub;     // interleave factor for nodes attaching to neigbour nodes
					int nbr_range = res/nsub; // number of neighbour vertices attaching to us - 1
					int subidxmask = (1<<dlvl)-1;
					int subidx = ilng & subidxmask;
					int nbr_ofs = subidx * nbr_range;
					nbr_elev += TILE_ELEVSTRIDE+1 + nbr_ofs + (res-line)*TILE_ELEVSTRIDE;
					double rad = mgr->CbodySize();
					// match nodes to neighbour elevations
					i0 = 0;
					i1 = nbr_range;
					if (keep_corner)
						if (ilng & 1) i1--; else i0++;
					for (i = i0; i <= i1; i++) {
						double radfac = (rad+nbr_elev[i])/(rad+elev[i*nsub]);
						mesh->vtx[vtx_ofs+i*vtx_skip].x = (float)(vtx_store[i*nsub].x*radfac + vtxshift.x*(radfac-1.0));
						mesh->vtx[vtx_ofs+i*vtx_skip].y = (float)(vtx_store[i*nsub].y*radfac + vtxshift.y*(radfac-1.0));
						mesh->vtx[vtx_ofs+i*vtx_skip].z = (float)(vtx_store[i*nsub].z*radfac + vtxshift.z*(radfac-1.0));
					}
					// interpolate the nodes that fall between neighbour nodes
					for (i = 0; i < nbr_range; i++)
						for (j = 1; j < nsub; j++)
							VtxInterpolate (mesh->vtx[vtx_ofs+i*vtx_skip+j], mesh->vtx[vtx_ofs+i*vtx_skip], mesh->vtx[vtx_ofs+(i+1)*vtx_skip], (double)j/double(nsub));
				}
			} else {
				// problems
			}
		}
	}
}

// -----------------------------------------------------------------------

void SurfTile::CreateLabels()
{
	if (!label) {
		label = TileLabel::Create(this);
	}
}

// -----------------------------------------------------------------------

inline void SurfTile::DeleteLabels()
{
	SAFE_DELETE(label);
}

// -----------------------------------------------------------------------

void SurfTile::RenderLabels(D3D9Pad *skp, oapi::Font **labelfont, int *fontidx)
{
	if (!label) return;
	label->Render(skp, labelfont, fontidx);
}


// =======================================================================
// =======================================================================

template<>
void TileManager2<SurfTile>::Render (MATRIX4 &dwmat, bool use_zbuf, const vPlanet::RenderPrm &rprm)
{
	// set generic parameters
	SetRenderPrm (dwmat, 0, use_zbuf, rprm);

	double np = 0.0, fp = 0.0;
	int i;
	class Scene *scene = GetClient()->GetScene();

	// adjust scaling parameters (can only be done if no z-buffering is in use)
	if (!use_zbuf) {
		double R = obj_size;
		double D = prm.cdist*R;
		double zmax = (D - R*R/D) * 1.5;
		double zmin = max (2.0, min (zmax*1e-4, (D-R) * 0.8));
	//double zscale = 1.0;

		np = scene->GetCameraNearPlane();
		fp = scene->GetCameraFarPlane();
		scene->SetCameraFrustumLimits (zmin, zmax);
	}

	// build a transformation matrix for frustum testing
	MATRIX4 Mproj = _MATRIX4(scene->GetProjectionMatrix());
	Mproj.m33 = 1.0; Mproj.m43 = -1.0;  // adjust near plane to 1, far plane to infinity
	MATRIX4 Mview = _MATRIX4(scene->GetViewMatrix());
	prm.dviewproj = mul(Mview,Mproj);

	// ---------------------------------------------------------------------

	static float spec_base = 0.7f; // 0.95f;

	// ---------------------------------------------------------------------
	// Initialize shading technique and feed planet specific data to shaders
	//
	Shader()->SetTechnique(eTileTech);

	HR(Shader()->SetBool(sbSpherical, false));

	if (ElevMode == eElevMode::Spherical) {
		// Force spherical rendering at shader level
		HR(Shader()->SetBool(sbSpherical, true));
	}	

	if (use_zbuf) {
		pDev->SetRenderState(D3DRS_ZENABLE, 1);
		pDev->SetRenderState(D3DRS_ZWRITEENABLE, 1);
	}
	else {
		pDev->SetRenderState(D3DRS_ZENABLE, 0);
		pDev->SetRenderState(D3DRS_ZWRITEENABLE, 0);
		HR(Shader()->SetBool(sbSpherical, true));
	}

	HR(Shader()->SetMatrix(smViewProj, scene->GetProjectionViewMatrix()));
	HR(Shader()->SetVector(svWater, &D3DXVECTOR4(spec_base*1.2f, spec_base*1.0f, spec_base*0.8f, 50.0f)));
	HR(Shader()->SetBool(sbEnvEnable, false));

	// -------------------------------------------------------------------
	vVessel *vFocus = scene->GetFocusVisual();
	vPlanet *vProxy = scene->GetCameraProxyVisual();

	// Setup Environment map ---------------------------------------------
	//
	if (vFocus && bEnvMapEnabled) {
		LPDIRECT3DCUBETEXTURE9 pEnv = vFocus->GetEnvMap(0);
		if (pEnv) {
			HR(Shader()->SetTexture(stEnvMap, pEnv));
			HR(Shader()->SetBool(sbEnvEnable, true));
		}
	}

	// Setup micro textures ---------------------------------------------
	//
	if (vp->MicroCfg.bEnabled) {
		HR(Shader()->SetTexture(stMicroA, vp->MicroCfg.Level[0].pTex));
		HR(Shader()->SetTexture(stMicroB, vp->MicroCfg.Level[1].pTex));
		HR(Shader()->SetTexture(stMicroC, vp->MicroCfg.Level[2].pTex));
		HR(Shader()->SetBool(sbMicroNormals, vp->MicroCfg.bNormals));
	}


	// ------------------------------------------------------------------
	// TODO: render full sphere for levels < 4

	loader->WaitForMutex();

	// update the tree
	for (i = 0; i < 2; i++)
		ProcessNode (tiletree+i);

	vp->tile_cache = NULL;

	if (scene->GetRenderPass() == RENDERPASS_MAINSCENE) ResetMinMaxElev();

	// render the tree
	for (i = 0; i < 2; i++)
		RenderNode (tiletree+i);

	loader->ReleaseMutex();


	// Backup the stats and clear counters
	if (scene->GetRenderPass() == RENDERPASS_MAINSCENE) prevstat = elvstat;
	elvstat.Elev = elvstat.Sphe = 0;

	/*if (scene->GetRenderPass() == RENDERPASS_MAINSCENE) {
		if (GetHandle() == scene->GetCameraNearBody()) {
			D3D9DebugLog("Body=%s, Elv=%d, Sph=%d", CbodyName(), prevstat.Elev, prevstat.Sphe);
			if (ElevMode == eElevMode::Elevated) D3D9DebugLog("Elevated");
			if (ElevMode == eElevMode::Spherical) D3D9DebugLog("Spherical");
			if (ElevMode == eElevMode::ForcedElevated) D3D9DebugLog("ForcedElevated");
		}
	}*/

	if (np)
		scene->SetCameraFrustumLimits(np,fp);
}

// -----------------------------------------------------------------------

template<>
void TileManager2<SurfTile>::RenderLabels(D3D9Pad *skp, oapi::Font **labelfont, int *fontidx)
{
	for (int i = 0; i < 2; ++i) {
		RenderNodeLabels(tiletree + i, skp, labelfont, fontidx);
	}
}

// -----------------------------------------------------------------------

template<>
void TileManager2<SurfTile>::CreateLabels()
{
	loader->WaitForMutex();
	for (int i = 0; i < 2; ++i) {
		SetSubtreeLabels(tiletree + i, true);
	}
	loader->ReleaseMutex();
}

// -----------------------------------------------------------------------

template<>
void TileManager2<SurfTile>::DeleteLabels()
{
	loader->WaitForMutex();
	for (int i = 0; i < 2; ++i) {
		SetSubtreeLabels(tiletree + i, false);
	}
	loader->ReleaseMutex();
}

// -----------------------------------------------------------------------

template<>
void TileManager2<SurfTile>::SetSubtreeLabels(QuadTreeNode<SurfTile> *node, bool activate)
{
	if (node->Entry()) {
		if (activate) node->Entry()->CreateLabels();
		else          node->Entry()->DeleteLabels();
	}
	for (int i = 0; i < 4; ++i) {
		if (node->Child(i)) {
			SetSubtreeLabels(node->Child(i), activate);
		}
	}
}

// -----------------------------------------------------------------------

template<>
void TileManager2<SurfTile>::LoadZTrees()
{
	treeMgr = new ZTreeMgr*[ntreeMgr = 5]();
	if (cprm.tileLoadFlags & 0x0002) {
		treeMgr[0] = ZTreeMgr::CreateFromFile(m_dataRootDir.c_str(), ZTreeMgr::LAYER_SURF);
		treeMgr[1] = ZTreeMgr::CreateFromFile(m_dataRootDir.c_str(), ZTreeMgr::LAYER_MASK);
		treeMgr[2] = ZTreeMgr::CreateFromFile(m_dataRootDir.c_str(), ZTreeMgr::LAYER_ELEV);
		treeMgr[3] = ZTreeMgr::CreateFromFile(m_dataRootDir.c_str(), ZTreeMgr::LAYER_ELEVMOD);
		treeMgr[4] = ZTreeMgr::CreateFromFile(m_dataRootDir.c_str(), ZTreeMgr::LAYER_LABEL);
	}
	else {
		for (int i = 0; i < ntreeMgr; i++)
			treeMgr[i] = 0;
	}
}

// -----------------------------------------------------------------------

template<>
void TileManager2<SurfTile>::InitHasIndividualFiles()
{
	hasIndividualFiles = new bool[ntreeMgr]();
	if (cprm.tileLoadFlags & 0x0001) {
		const char *name[] = { "Surf", "Mask", "Elev", "Elev_mod", "Label" };
		char path[MAX_PATH], dummy[MAX_PATH];
		for (int i = 0; i < ARRAYSIZE(name); ++i) {
			sprintf_s(path, MAX_PATH, "%s\\%s", m_dataRootDir.c_str(), name[i]);
			hasIndividualFiles[i] = FileExists(path);
		}
	}
}

// -----------------------------------------------------------------------

template<>
Tile * TileManager2<SurfTile>::SearchTile (double lng, double lat, int maxlvl, bool bOwntex) const
{
	if (lng<0) return SearchTileSub(&tiletree[0], lng, lat, maxlvl, bOwntex);
	else	   return SearchTileSub(&tiletree[1], lng, lat, maxlvl, bOwntex);
}
// -----------------------------------------------------------------------

template<>
int TileManager2<SurfTile>::GetElevation(double lng, double lat, double *elev, FVECTOR3 *nrm, SurfTile **cache)
{
	int rv = 0;
	loader->WaitForMutex();
	if (lng<0) rv = tiletree[0].Entry()->GetElevation(lng, lat, elev, nrm, cache);
	else	   rv = tiletree[1].Entry()->GetElevation(lng, lat, elev, nrm, cache);
	loader->ReleaseMutex();
	return rv;
}

// -----------------------------------------------------------------------

template<>
void TileManager2<SurfTile>::Pick(D3DXVECTOR3 &vRay, TILEPICK *pPick)
{
	if (bFreeze) return;

	std::list<Tile *> tiles;

	pPick->d = 1e12f;

	// Give me a list of rendered tiles ----------------------------------------------
	//
	QueryTiles(&tiletree[0], tiles);
	QueryTiles(&tiletree[1], tiles);

	for each (Tile * tile in tiles)	tile->Pick(&(tile->mWorld), &vRay, *pPick);
}

// -----------------------------------------------------------------------

template<>
SURFHANDLE TileManager2<SurfTile>::SeekTileTexture(int iLng, int iLat, int level, int flags)
{
	bool bOk = false;
	LPDIRECT3DTEXTURE9 pTex = NULL;
		
	if (flags & gcTileFlags::TEXTURE)
	{
		if (flags & gcTileFlags::CACHE)
		{
			char path[MAX_PATH];
			sprintf_s(path, MAX_PATH, "%s\\Surf\\%02d\\%06d\\%06d.dds", m_dataRootDir.c_str(), level + 4, iLat, iLng);
			bOk = (D3DXCreateTextureFromFileEx(Dev(), path, 0, 0, 0, 0, D3DFMT_FROM_FILE, D3DPOOL_DEFAULT,
				D3DX_FILTER_NONE, D3DX_FILTER_BOX, 0, NULL, NULL, &pTex) == S_OK);
		}

		if (flags & gcTileFlags::TREE)
		{
			if (!bOk) {
				if (ZTreeManager(0)) {
					BYTE *buf;
					DWORD ndata = ZTreeManager(0)->ReadData(level + 4, iLat, iLng, &buf);
					if (ndata) {
						if (D3DXCreateTextureFromFileInMemoryEx(Dev(), buf, ndata, 0, 0, D3DX_FROM_FILE, 0,
							D3DFMT_FROM_FILE, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_BOX, 0, NULL, NULL, &pTex) == S_OK) bOk = true;
						ZTreeManager(0)->ReleaseData(buf);
					}
				}
			}
		}
	}

	if (flags & gcTileFlags::MASK)
	{
		if (flags & gcTileFlags::CACHE)
		{
			char path[MAX_PATH];
			sprintf_s(path, MAX_PATH, "%s\\Mask\\%02d\\%06d\\%06d.dds", m_dataRootDir.c_str(), level + 4, iLat, iLng);
			bOk = (D3DXCreateTextureFromFileEx(Dev(), path, 0, 0, 0, 0, D3DFMT_FROM_FILE, D3DPOOL_DEFAULT,
				D3DX_FILTER_NONE, D3DX_FILTER_BOX, 0, NULL, NULL, &pTex) == S_OK);
		}

		if (flags & gcTileFlags::TREE)
		{
			if (!bOk) {
				if (ZTreeManager(1)) {
					BYTE* buf;
					DWORD ndata = ZTreeManager(1)->ReadData(level + 4, iLat, iLng, &buf);
					if (ndata) {
						if (D3DXCreateTextureFromFileInMemoryEx(Dev(), buf, ndata, 0, 0, D3DX_FROM_FILE, 0,
							D3DFMT_FROM_FILE, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_BOX, 0, NULL, NULL, &pTex) == S_OK) bOk = true;
						ZTreeManager(1)->ReleaseData(buf);
					}
				}
			}
		}
	}

	if (bOk && pTex) return new SurfNative(pTex, OAPISURFACE_TEXTURE);
	return NULL;
}

// -----------------------------------------------------------------------

template<>
bool TileManager2<SurfTile>::HasTileData(int iLng, int iLat, int level, int flags)
{
	bool bOk = false;
	
	if (flags & gcTileFlags::TEXTURE) {
		if (flags & gcTileFlags::CACHE) {
			char path[MAX_PATH];
			sprintf_s(path, MAX_PATH, "%s\\Surf\\%02d\\%06d\\%06d.dds", m_dataRootDir.c_str(), level + 4, iLat, iLng);
			bOk = FileExists(path);

		}
		if (flags & gcTileFlags::TREE) if (!bOk && ZTreeManager(0)) if (ZTreeManager(0)->Idx(level + 4, iLat, iLng) != ((DWORD)-1)) bOk = true;
	}

	if (flags & gcTileFlags::MASK) {
		if (flags & gcTileFlags::CACHE) {
			char path[MAX_PATH];
			sprintf_s(path, MAX_PATH, "%s\\Mask\\%02d\\%06d\\%06d.dds", m_dataRootDir.c_str(), level + 4, iLat, iLng);
			bOk = FileExists(path);
		}
		if (flags & gcTileFlags::TREE) if (!bOk && ZTreeManager(1)) if (ZTreeManager(1)->Idx(level + 4, iLat, iLng) != ((DWORD)-1)) bOk = true;
	}

	if (flags & gcTileFlags::ELEVATION) {
		if (flags & gcTileFlags::CACHE) {
			char path[MAX_PATH];
			sprintf_s(path, MAX_PATH, "%s\\Elev\\%02d\\%06d\\%06d.elv", m_dataRootDir.c_str(), level + 4, iLat, iLng);
			bOk = FileExists(path);
		}
		if (flags & gcTileFlags::TREE) if (!bOk && ZTreeManager(2)) if (ZTreeManager(2)->Idx(level + 4, iLat, iLng) != ((DWORD)-1)) bOk = true;
	}

	return bOk;
}

// -----------------------------------------------------------------------

template<>
float* TileManager2<SurfTile>::BrowseElevationData(int lvl, int ilat, int ilng, int flags, ELEVFILEHEADER* _hdr)
{
	ELEVFILEHEADER ehdr;
	const int ndat = TILE_ELEVSTRIDE * TILE_ELEVSTRIDE;
	float* elev = NULL;
	char path[MAX_PATH];
	char fname[128];
	FILE* f;
	int i;

	// Elevation data
	if (flags & gcTileFlags::CACHE) { // try loading from individual tile file
		sprintf_s(path, MAX_PATH, "%s\\Elev\\%02d\\%06d\\%06d.elv", m_dataRootDir.c_str(), lvl + 4, ilat, ilng);
		if (!fopen_s(&f, path, "rb")) {
			elev = new float[ndat];
			fread(&ehdr, sizeof(ELEVFILEHEADER), 1, f);
			if (ehdr.hdrsize != sizeof(ELEVFILEHEADER)) fseek(f, ehdr.hdrsize, SEEK_SET);

			ehdr.scale = 1.0;

			switch (ehdr.dtype) {
			case 0: // flat tile, defined by offset
				for (i = 0; i < ndat; i++) elev[i] = 0.0f;
				break;
			case 8: {
				UINT8* tmp = new UINT8[ndat];
				fread(tmp, sizeof(UINT8), ndat, f);
				for (i = 0; i < ndat; i++) elev[i] = float(tmp[i]);
				delete[]tmp;
				break;
			}
			case -16: {
				INT16* tmp = new INT16[ndat];
				fread(tmp, sizeof(INT16), ndat, f);
				for (i = 0; i < ndat; i++) elev[i] = float(tmp[i]);
				delete[]tmp;
				break;
			}
			}
			fclose(f);
		}
	}
	if (!elev && (flags & gcTileFlags::TREE) && ZTreeManager(2)) { // try loading from compressed archive
		BYTE* buf;
		DWORD ndata = ZTreeManager(2)->ReadData(lvl + 4, ilat, ilng, &buf);
		if (ndata) {
			BYTE* p = buf;
			elev = new float[ndat];
			memcpy(&ehdr, p, sizeof(ELEVFILEHEADER));
			p += ehdr.hdrsize;
			INT16* pi = (INT16*)p;
			switch (ehdr.dtype) {
			case 0:
				for (i = 0; i < ndat; i++) elev[i] = 0.0f; break;
			case 8:
				for (i = 0; i < ndat; i++) elev[i] = float(*p++); break;
			case -16:
				for (i = 0; i < ndat; i++) elev[i] = float(*pi++); break;
			}
			ZTreeManager(2)->ReleaseData(buf);
		}
	}

	if (elev) {
		if (ehdr.scale != 1.0) for (i = 0; i < ndat; i++) elev[i] = trunc(elev[i] * float(ehdr.scale));
		if (ehdr.offset) for (i = 0; i < ndat; i++)	elev[i] += trunc(float(ehdr.offset));
	}


	// Elevation mod data
	if (elev && (flags & gcTileFlags::MOD)) {
		bool ok = false;
		ELEVFILEHEADER hdr;
		if (flags & gcTileFlags::CACHE) { // try loading from individual tile file
			sprintf_s(fname, ARRAYSIZE(fname), "%s\\Elev_mod\\%02d\\%06d\\%06d.elv", CbodyName(), lvl + 4, ilat, ilng);
			bool found = GetClient()->TexturePath(fname, path);
			if (found && !fopen_s(&f, path, "rb")) {

				fread(&hdr, sizeof(ELEVFILEHEADER), 1, f);
				if (hdr.hdrsize != sizeof(ELEVFILEHEADER)) fseek(f, hdr.hdrsize, SEEK_SET);

				switch (hdr.dtype)
				{
				case 0: // overwrite the entire tile with a flat offset
					for (i = 0; i < ndat; i++) elev[i] = float(hdr.offset);
					break;

				case 8: {
					const UINT8 mask = UCHAR_MAX;
					UINT8* tmp = new UINT8[ndat];
					fread(tmp, sizeof(UINT8), ndat, f);
					for (i = 0; i < ndat; i++)
						if (tmp[i] != mask)
							elev[i] = float(trunc(float(tmp[i]) * hdr.scale) + trunc(hdr.offset));
					delete[]tmp;
					break;
				}
				case -16: {
					const INT16 mask = SHRT_MAX;
					INT16* tmp = new INT16[ndat];
					fread(tmp, sizeof(INT16), ndat, f);
					for (i = 0; i < ndat; i++)
						if (tmp[i] != mask)
							elev[i] = float(trunc(float(tmp[i]) * hdr.scale) + trunc(hdr.offset));
					delete[]tmp;
					break;
				}
				}
				fclose(f);
				ok = true;
			}
		}

		if (!ok && (flags & gcTileFlags::TREE) && ZTreeManager(3)) { // try loading from compressed archive
			BYTE* buf;
			DWORD ndata = ZTreeManager(3)->ReadData(lvl + 4, ilat, ilng, &buf);
			if (ndata) {
				BYTE* p = buf;
				ELEVFILEHEADER* phdr = (ELEVFILEHEADER*)p;
				p += phdr->hdrsize;

				switch (phdr->dtype)
				{
				case 0:
					for (i = 0; i < ndat; i++) elev[i] = float(phdr->offset);
					break;
				case 8: {
					const UINT8 mask = UCHAR_MAX;
					for (i = 0; i < ndat; i++)
						if (p[i] != mask)
							elev[i] = float(trunc(float(p[i]) * phdr->scale) + trunc(phdr->offset));
					break;
				}
				case -16: {
					const INT16 mask = SHRT_MAX;
					INT16* buf16 = (INT16*)p;
					for (i = 0; i < ndat; i++)
						if (buf16[i] != mask)
							elev[i] = float(trunc(float(buf16[i]) * phdr->scale) + trunc(phdr->offset));
					break;
				}
				}
				ZTreeManager(3)->ReleaseData(buf);
			}
		}
	}

	if (_hdr) memcpy(_hdr, &ehdr, sizeof(ELEVFILEHEADER));

	return elev;
}
