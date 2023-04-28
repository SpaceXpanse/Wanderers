// Copyright (c) Martin Schweiger
// Licensed under the MIT License

// =======================================================================
// SpaceXpanseGraphics class
// Inline graphics client
// =======================================================================

#include "D3dmath.h"
#include "OGraphics.h"
#include "SpaceXpanse.h"
#include "Psys.h"
#include "Launchpad.h"
#include "ExtraRender.h"
#include "Scene.h"
#include "Camera.h"
#include "Particle.h"
#include "VVessel.h"
#include "Texture.h"
#include "TileMgr.h"
#include "tilemgr2.h"
#include "CSphereMgr.h"
#include "Log.h"
#include "Util.h"
#include "cryptstring.h"
#include "resource.h"
#include "resource_video.h"

extern Camera *g_camera;
extern PlanetarySystem *g_psys;
extern TextureManager *g_texmanager;
extern char DBG_MSG[256];

TextureManager2 *g_texmanager2 = 0; // TODO: make member of SpaceXpanseGraphics

// =======================================================================

VideoTab::VideoTab (SpaceXpanseGraphics *og)
{
	gclient = og;
	cfg = og->spacexpanse->Cfg();
	hVid = og->hVid;
}

// =======================================================================

void VideoTab::Init ()
{
	char cbuf[20];
	DWORD i, w, h, ndev;
	D3D7Enum_DeviceInfo *devlist;
	D3D7Enum_GetDevices (&devlist, &ndev);
	D3D7Enum_DeviceInfo *dev = gclient->GetDeviceInfo();

	SendDlgItemMessage (hVid, IDC_VID_DEVICE, CB_RESETCONTENT, 0, 0);
	for (i = 0; i < ndev; i++) {
		SendMessage (GetDlgItem (hVid, IDC_VID_DEVICE), CB_ADDSTRING, 0,
			(LPARAM)(devlist[i].strDesc));
	}
	SendDlgItemMessage (hVid, IDC_VID_DEVICE, CB_SELECTSTRING, -1, (LPARAM)dev->strDesc);
	w = cfg->CfgDevPrm.WinW, h = cfg->CfgDevPrm.WinH;
	SetWindowText (GetDlgItem (hVid, IDC_VID_WIDTH),  _itoa (w, cbuf, 10));
	SetWindowText (GetDlgItem (hVid, IDC_VID_HEIGHT), _itoa (h, cbuf, 10));
	if (w == (4*h)/3 || h == (3*w)/4)
		aspect_idx = 1;
	else if (w == (16*h)/10 || h == (10*w)/16)
		aspect_idx = 2;
	else if (w == (16*h)/9 || h == (9*w)/16)
		aspect_idx = 3;
	else
		aspect_idx = 0;
	SendDlgItemMessage (hVid, IDC_VID_ASPECT, BM_SETCHECK, aspect_idx ? BST_CHECKED : BST_UNCHECKED, 0);
	if (aspect_idx) aspect_idx--;
	SendDlgItemMessage (hVid, IDC_VID_4X3+aspect_idx, BM_SETCHECK, BST_CHECKED, 0);
	SendDlgItemMessage (hVid, IDC_VID_ENUM, BM_SETCHECK, cfg->CfgDevPrm.bForceEnum ? BST_CHECKED : BST_UNCHECKED, 0);
	SendDlgItemMessage (hVid, IDC_VID_VSYNC, BM_SETCHECK, cfg->CfgDevPrm.bNoVsync ? BST_CHECKED : BST_UNCHECKED, 0);
	SendDlgItemMessage (hVid, IDC_VID_PAGEFLIP, BM_SETCHECK, cfg->CfgDevPrm.bPageflip ? BST_UNCHECKED : BST_CHECKED, 0);
	SendDlgItemMessage (hVid, IDC_VID_STENCIL, BM_SETCHECK, cfg->CfgDevPrm.bTryStencil ? BST_CHECKED : BST_UNCHECKED, 0);

	DeviceChanged (dev);
}

// =======================================================================

void VideoTab::DeviceChanged (D3D7Enum_DeviceInfo *dev)
{
	char cbuf[256];
	DWORD i, j;
	DDSURFACEDESC2 &cmode = dev->ddsdFullscreenMode;
	DWORD nres = 0, *wres = new DWORD[dev->dwNumModes], *hres = new DWORD[dev->dwNumModes]; TRACENEW
	DWORD nbpp = 0, *bpp = new DWORD[dev->dwNumModes]; TRACENEW
	SendDlgItemMessage (hVid, IDC_VID_MODE, CB_RESETCONTENT, 0, 0);
	SendDlgItemMessage (hVid, IDC_VID_BPP, CB_RESETCONTENT, 0, 0);
	for (i = 0; i < dev->dwNumModes; i++) {
		DDSURFACEDESC2 *ddsd = dev->pddsdModes+i;
		DWORD w = ddsd->dwWidth, h = ddsd->dwHeight;
		for (j = 0; j < nres; j++) if (wres[j] == w && hres[j] == h) break;
		if (j == nres) wres[nres] = w, hres[nres] = h, nres++;
		DWORD bc = ddsd->ddpfPixelFormat.dwRGBBitCount;
		for (j = 0; j < nbpp; j++) if (bpp[j] == bc) break;
		if (j == nbpp) bpp[nbpp++] = bc;
	}
	for (i = 0; i < nres; i++) {
		sprintf (cbuf, "%d x %d", wres[i], hres[i]);
		SendDlgItemMessage (hVid, IDC_VID_MODE, CB_ADDSTRING, 0, (LPARAM)cbuf);
		SendDlgItemMessage (hVid, IDC_VID_MODE, CB_SETITEMDATA, i, (LPARAM)(hres[i]<<16 | wres[i]));
		if (wres[i] == cmode.dwWidth && hres[i] == cmode.dwHeight)
			SendDlgItemMessage (hVid, IDC_VID_MODE, CB_SETCURSEL, i, 0);
	}
	for (i = 0; i < nbpp; i++) {
		sprintf (cbuf, "%d", bpp[i]);
		SendDlgItemMessage (hVid, IDC_VID_BPP, CB_ADDSTRING, 0, (LPARAM)cbuf);
		SendDlgItemMessage (hVid, IDC_VID_BPP, CB_SETITEMDATA, i, (LPARAM)(bpp[i]));
		if (bpp[i] == cmode.ddpfPixelFormat.dwRGBBitCount)
			SendDlgItemMessage (hVid, IDC_VID_BPP, CB_SETCURSEL, i, 0);
	}
	for (i = 0; i < 2; i++)
		EnableWindow (GetDlgItem (hVid, IDC_VID_FULL+i), TRUE);
	SendDlgItemMessage (hVid, dev->bWindowed ? IDC_VID_WINDOW:IDC_VID_FULL, BM_CLICK, 0, 0);
	for (i = 0; i < 2; i++)
		EnableWindow (GetDlgItem (hVid, IDC_VID_FULL+i), dev->bDesktopCompatible);
	delete []wres;
	wres = NULL;
	delete []hres;
	hres = NULL;
	delete []bpp;
	bpp = NULL;
}

// =======================================================================

void VideoTab::ModeChanged (D3D7Enum_DeviceInfo *dev, DWORD idx)
{
	DWORD i, data, w, h, mode, bpp, ibpp, usebpp;
	data = SendDlgItemMessage (hVid, IDC_VID_MODE, CB_GETITEMDATA, idx, 0);
	w    = data & 0xFFFF;
	h    = data >> 16;
	// check that this resolution is compatible with the current bpp setting
	idx  = SendDlgItemMessage (hVid, IDC_VID_BPP, CB_GETCURSEL, 0, 0);
	bpp  = SendDlgItemMessage (hVid, IDC_VID_BPP, CB_GETITEMDATA, idx, 0);
	for (i = mode = usebpp = 0; i < dev->dwNumModes; i++) {
		DDSURFACEDESC2 *ddsd = dev->pddsdModes+i;
		if (ddsd->dwWidth != w || ddsd->dwHeight != h) continue;
		ibpp = ddsd->ddpfPixelFormat.dwRGBBitCount;
		if (ibpp == bpp)   { usebpp = ibpp; mode = i; break; } // found match
		if (ibpp > usebpp) { usebpp = ibpp; mode = i; } // best match so far
	}
	dev->dwCurrentMode = mode;
	dev->ddsdFullscreenMode = dev->pddsdModes[dev->dwCurrentMode];
	// if a bpp change was required, notify the bpp control
	if (bpp != usebpp) {
		char cbuf[20];
		SendDlgItemMessage (hVid, IDC_VID_BPP, CB_SELECTSTRING, -1,
			(LPARAM)_itoa (usebpp, cbuf, 10));
	}
}

// =======================================================================

void VideoTab::BPPChanged (D3D7Enum_DeviceInfo *dev, DWORD idx)
{
	DWORD data, w, h, mode, bpp;
	bpp  = SendDlgItemMessage (hVid, IDC_VID_BPP, CB_GETITEMDATA, idx, 0);
	// check that this bitdepth is compatible with the current resolution
	idx  = SendDlgItemMessage (hVid, IDC_VID_MODE, CB_GETCURSEL, 0, 0);
	data = SendDlgItemMessage (hVid, IDC_VID_MODE, CB_GETITEMDATA, idx, 0);
	w    = data & 0xFFFF;
	h    = data >> 16;
	for (mode = 0; mode < dev->dwNumModes; mode++) {
		DDSURFACEDESC2 *ddsd = dev->pddsdModes+mode;
		if (ddsd->ddpfPixelFormat.dwRGBBitCount == bpp &&
			ddsd->dwWidth == w &&
			ddsd->dwHeight == h) {
				dev->dwCurrentMode = mode;
				dev->ddsdFullscreenMode = dev->pddsdModes[mode];
				return;
		}
	}

	// If we get here, the selected screen resolution isn't supported
	// at this bit depth (shouldn't happen)
	bpp = dev->pddsdModes[dev->dwCurrentMode].ddpfPixelFormat.dwRGBBitCount;
	char cbuf[20];
	SendDlgItemMessage (hVid, IDC_VID_BPP, CB_SELECTSTRING, -1,
		(LPARAM)_itoa (bpp, cbuf, 10));

	// If we get here, then we've screwed up big time
	// and leave quietly
}

// =======================================================================

void VideoTab::DispmodeChanged (D3D7Enum_DeviceInfo *dev, BOOL bWindow)
{
	DWORD i;
	for (i = 0; i < 6; i++)
		EnableWindow (GetDlgItem (hVid, IDC_VID_STATIC5+i), !bWindow);
	for (i = 0; i < 9; i++)
		EnableWindow (GetDlgItem (hVid, IDC_VID_STATIC7+i), bWindow);
	if (!bWindow) {
		if (SendDlgItemMessage (hVid, IDC_VID_PAGEFLIP, BM_GETCHECK, 0, 0) == BST_CHECKED)
			EnableWindow (GetDlgItem (hVid, IDC_VID_VSYNC), FALSE);
	} else {
		if (SendDlgItemMessage (hVid, IDC_VID_ASPECT, BM_GETCHECK, 0, 0) != BST_CHECKED) {
			for (i = 0; i < 3; i++)
				EnableWindow (GetDlgItem (hVid, IDC_VID_4X3+i), FALSE);
		}
	}
		
	dev->bWindowed = bWindow;
}

// =======================================================================

void VideoTab::PageFlipChanged ()
{
	bool disable_pageflip = (SendDlgItemMessage (hVid, IDC_VID_PAGEFLIP, BM_GETCHECK, 0, 0) == BST_CHECKED);
	EnableWindow (GetDlgItem (hVid, IDC_VID_VSYNC), disable_pageflip ? FALSE:TRUE);
}

// =======================================================================

void VideoTab::FixedAspectChanged ()
{
	bool fixed_aspect = (SendDlgItemMessage (hVid, IDC_VID_ASPECT, BM_GETCHECK, 0, 0) == BST_CHECKED);
	for (int i = 0; i < 3; i++)
		EnableWindow (GetDlgItem (hVid, IDC_VID_4X3+i), fixed_aspect ? TRUE:FALSE);
}

// =======================================================================

static int aspect_wfac[4] = {4,16,16};
static int aspect_hfac[4] = {3,10,9};

void VideoTab::WidthChanged ()
{
	if (SendDlgItemMessage (hVid, IDC_VID_ASPECT, BM_GETCHECK, 0, 0) == BST_CHECKED) {
		char cbuf[128];
		int res, w, h, wfac = aspect_wfac[aspect_idx], hfac = aspect_hfac[aspect_idx];
		GetWindowText (GetDlgItem (hVid, IDC_VID_WIDTH), cbuf, 127);
		res = sscanf (cbuf, "%d", &w);
		if (res != 1) w = 0;
		GetWindowText (GetDlgItem (hVid, IDC_VID_HEIGHT), cbuf, 127);
		res = sscanf (cbuf, "%d", &h);
		if (res != 1) h = 0;
		if (w != (wfac*h)/hfac) {
			h = (hfac*w)/wfac;
			SetWindowText (GetDlgItem (hVid, IDC_VID_HEIGHT), _itoa (h, cbuf, 10));
		}
	}
}

// =======================================================================

void VideoTab::HeightChanged ()
{
	if (SendDlgItemMessage (hVid, IDC_VID_ASPECT, BM_GETCHECK, 0, 0) == BST_CHECKED) {
		char cbuf[128];
		int res, w, h, wfac = aspect_wfac[aspect_idx], hfac = aspect_hfac[aspect_idx];
		GetWindowText (GetDlgItem (hVid, IDC_VID_HEIGHT), cbuf, 127);
		res = sscanf (cbuf, "%d", &h);
		if (res != 1) h = 0;
		GetWindowText (GetDlgItem (hVid, IDC_VID_WIDTH),  cbuf, 127);
		res = sscanf (cbuf, "%d", &w);
		if (res != 1) w = 0;
		if (h != (hfac*w)/wfac) {
			w = (wfac*h)/hfac;
			SetWindowText (GetDlgItem (hVid, IDC_VID_WIDTH), _itoa (w, cbuf, 10));
		}
	}
}

// =======================================================================

void VideoTab::ForceDeviceEnum ()
{
	if (SendDlgItemMessage (hVid, IDC_VID_ENUM, BM_GETCHECK, 0, 0) == BST_CHECKED) {
		gclient->ReEnumerate ("Device.dat", 0);
		cfg->CfgDevPrm.bForceEnum = true;
		Init ();
	}
}

// =======================================================================

INT_PTR VideoTab::WndProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_VID_DEVICE:
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				DWORD idx = SendDlgItemMessage (hWnd, IDC_VID_DEVICE, CB_GETCURSEL, 0, 0);
				gclient->SetDeviceInfo (D3D7Enum_GetDevice (idx));
				DeviceChanged (D3D7Enum_GetDevice (idx));
				return TRUE;
			}
			break;
		case IDC_VID_MODE:
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				DWORD idx = SendDlgItemMessage (hWnd, IDC_VID_MODE, CB_GETCURSEL, 0, 0);
				ModeChanged (gclient->GetDeviceInfo(), idx);
				return TRUE;
			}
			break;
		case IDC_VID_BPP:
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				DWORD idx = SendDlgItemMessage (hWnd, IDC_VID_BPP, CB_GETCURSEL, 0, 0);
				BPPChanged (gclient->GetDeviceInfo(), idx);
				return TRUE;
			}
			break;
		case IDC_VID_FULL:
			if (HIWORD(wParam) == BN_CLICKED) {
				DispmodeChanged (gclient->GetDeviceInfo(), FALSE);
				return TRUE;
			}
			break;
		case IDC_VID_WINDOW:
			if (HIWORD(wParam) == BN_CLICKED) {
				DispmodeChanged (gclient->GetDeviceInfo(), TRUE);
				return TRUE;
			}
			break;
		case IDC_VID_PAGEFLIP:
			if (HIWORD(wParam) == BN_CLICKED) {
				PageFlipChanged();
				return TRUE;
			}
			break;
		case IDC_VID_WIDTH:
			if (HIWORD(wParam) == EN_CHANGE) {
				WidthChanged ();
				return TRUE;
			}
			break;
		case IDC_VID_HEIGHT:
			if (HIWORD(wParam) == EN_CHANGE) {
				HeightChanged ();
				return TRUE;
			}
			break;
		case IDC_VID_ASPECT:
			if (HIWORD(wParam) == BN_CLICKED) {
				FixedAspectChanged ();
				WidthChanged ();
				return TRUE;
			}
			break;
		case IDC_VID_4X3:
		case IDC_VID_16X10:
		case IDC_VID_16X9:
			if (HIWORD(wParam) == BN_CLICKED) {
				aspect_idx = LOWORD(wParam)-IDC_VID_4X3;
				WidthChanged();
				return TRUE;
			}
			break;
		case IDC_VID_ENUM:
			if (HIWORD(wParam) == BN_CLICKED) {
				ForceDeviceEnum ();
				return TRUE;
			}
			break;
		}
		break;
	}
	return FALSE;
}

// =======================================================================
// =======================================================================

SpaceXpanseGraphics::SpaceXpanseGraphics (SpaceXpanse *po): GDIClient (po->GetInstance())
{
	spacexpanse                = po;
	scene                  = NULL;
	vtab                   = NULL;
	clipper                = NULL;
    m_pFramework           = nullptr;
	m_pDD                  = nullptr;
	m_pD3D                 = nullptr;
	m_pD3DDevice           = nullptr;
	m_pDeviceInfo          = nullptr;
	m_pddsRenderTarget     = NULL;
	m_pddsRenderTargetLeft = NULL;
	viewW = viewH          = 0;
	viewBPP                = 0;
	bFullscreen            = false;
	bUseZBuffer            = true;
	bUseStereo             = false;
	bNoVSync               = false;
	lstatus.bkgDC          = 0;

	m_lpiGroup               = nullptr;
	m_lpiPlanetRenderOptions = nullptr;
}

// =======================================================================

SpaceXpanseGraphics::~SpaceXpanseGraphics ()
{
	if (vtab) delete vtab;
	if (scene) delete scene;
	if (m_pFramework) delete m_pFramework;
	D3D7Enum_FreeResources ();

	// The following resources should already have been freed at this point

	if (GetPenCount())
		LOGOUT_WARN("Sketchpad pens still allocated: %d", GetPenCount());
	if (GetBrushCount())
		LOGOUT_WARN("Sketchpad brushes still allocated: %d", GetBrushCount());
	if (GetFontCount())
		LOGOUT_WARN("Sketchpad fonts still allocated: %d", GetFontCount());

	if (m_lpiGroup) {
		LOGOUT_WARN("Launchpad item still allocated: m_lpiGroup");
		delete m_lpiGroup;
	}
	if (m_lpiPlanetRenderOptions) {
		LOGOUT_WARN("Launchpad item still allocated: m_lpiPlanetRenderOptions");
		delete m_lpiPlanetRenderOptions;
	}
}

// =======================================================================

bool SpaceXpanseGraphics::SelectDevice (D3D7Enum_DeviceInfo **dev, DeviceData *dd)
{
	D3D7Enum_DeviceInfo *devlist, *dv;
	DWORD ndev;
	D3D7Enum_GetDevices (&devlist, &ndev);

	if (!dd || dd->dwDevice < 0 || dd->dwDevice >= ndev)
		return false;
	dv = devlist + dd->dwDevice;

	if (dd->dwMode >= 0 && dd->dwMode < dv->dwNumModes)
		dv->dwCurrentMode = dd->dwMode;
	else
		dv->dwCurrentMode = 0;
	dv->ddsdFullscreenMode = dv->pddsdModes[dv->dwCurrentMode];

	if (!dd->bFullscreen && dv->bDesktopCompatible)
		dv->bWindowed = TRUE;
	else
		dv->bWindowed = FALSE;

	if (!dd->bStereo && dv->bStereoCompatible)
		dv->bStereo = TRUE;
	else
		dv->bStereo = FALSE;

	*dev = dv;
	return true;
}

// =======================================================================

HRESULT SpaceXpanseGraphics::ConfirmDevice (DDCAPS*, D3DDEVICEDESC7*)
{
	// put checks in here
	return S_OK;
}

// =======================================================================

HRESULT SpaceXpanseGraphics::ReEnumerate (TCHAR* fname, DeviceData *dd)
{
	HRESULT hr;
	D3D7Enum_FreeResources ();

	if (FAILED (hr = D3D7Enum_EnumerateDevices (ConfirmDevice))) {
		LOGOUT_DDERR (hr);
		return hr;
	}
	if (fname) D3D7Enum_WriteDeviceList (fname);

	if (!SelectDevice (&m_pDeviceInfo, dd))
		if (FAILED (hr = D3D7Enum_SelectDefaultDevice (&m_pDeviceInfo))) {
			LOGOUT_DDERR (hr);
			return hr;
		}
	return S_OK;
}

// =======================================================================

void SpaceXpanseGraphics::clbkRefreshVideoData ()
{
	char cbuf[128];
	int res;
	DWORD i, dat, w, h, bpp, ndev, nmod;
	VIDEODATA *vd = GetVideoData();
	HWND hVT = LaunchpadVideoTab();

	D3D7Enum_DeviceInfo *devlist, *dev;
	D3D7Enum_GetDevices (&devlist, &ndev);

	// device parameters
	i   = SendDlgItemMessage (hVT, IDC_VID_DEVICE, CB_GETCURSEL, 0, 0);
	if (i >= ndev) i = 0; // should not happen
	dev = devlist+i;
	vd->deviceidx = i;
	i   = SendDlgItemMessage (hVT, IDC_VID_MODE, CB_GETCURSEL, 0, 0);
	dat = SendDlgItemMessage (hVT, IDC_VID_MODE, CB_GETITEMDATA, i, 0);
	w   = dat & 0xFFFF;
	h   = dat >> 16;
	i   = SendDlgItemMessage (hVT, IDC_VID_BPP, CB_GETCURSEL, 0, 0);
	bpp = SendDlgItemMessage (hVT, IDC_VID_BPP, CB_GETITEMDATA, i, 0);
	nmod = dev->dwNumModes;
	vd->modeidx = 0; // in case there is a problem
	for (i = 0; i < nmod; i++) {
		if (dev->pddsdModes[i].dwWidth == w && dev->pddsdModes[i].dwHeight == h &&
			dev->pddsdModes[i].ddpfPixelFormat.dwRGBBitCount == bpp) {
			vd->modeidx = i;
			break;
		}
	}

	vd->fullscreen = (SendDlgItemMessage (hVT, IDC_VID_FULL, BM_GETCHECK, 0, 0) == BST_CHECKED);
	vd->novsync = (SendDlgItemMessage (hVT, IDC_VID_VSYNC, BM_GETCHECK, 0, 0) == BST_CHECKED);
	vd->pageflip = (SendDlgItemMessage (hVT, IDC_VID_PAGEFLIP, BM_GETCHECK, 0, 0) != BST_CHECKED);
	vd->trystencil = (SendDlgItemMessage (hVT, IDC_VID_STENCIL, BM_GETCHECK, 0, 0) == BST_CHECKED);
	vd->forceenum = (SendDlgItemMessage (hVT, IDC_VID_ENUM, BM_GETCHECK, 0, 0) == BST_CHECKED);
	GetWindowText (GetDlgItem (hVT, IDC_VID_WIDTH),  cbuf, 127); 
	res = sscanf (cbuf, "%d", &vd->winw);
	if (res != 1 || vd->winw < 400) {
		vd->winw = 400;
		SetWindowText (GetDlgItem (hVT, IDC_VID_WIDTH), _itoa(vd->winw, cbuf, 10));
	}
	GetWindowText (GetDlgItem (hVT, IDC_VID_HEIGHT), cbuf, 127); 
	res = sscanf (cbuf, "%d", &vd->winh);
	if (res != 1 || vd->winh < 300) {
		vd->winh = 300;
		SetWindowText (GetDlgItem (hVT, IDC_VID_HEIGHT), _itoa(vd->winh, cbuf, 10));
	}
}

// =======================================================================

bool SpaceXpanseGraphics::clbkInitialise ()
{
	if (!GraphicsClient::clbkInitialise()) return false;

	static char *DevName = "Device.dat";
	Config *pcfg = spacexpanse->Cfg();
	bool force = pcfg->CfgDevPrm.bForceEnum;

    HRESULT hr;

	if (force || !D3D7Enum_ReadDeviceList (DevName)) {
	    // The callback is used so the app can confirm/reject each enumerated
		// device depending on its capabilities.
	    if (FAILED (hr = D3D7Enum_EnumerateDevices (ConfirmDevice))) {
			LOGOUT_DDERR (hr);
			return false;
		}
		D3D7Enum_WriteDeviceList (DevName);
	}

    // Select a device using user parameters from config file.
	DeviceData dd = {
		pcfg->CfgDevPrm.Device_idx,
		pcfg->CfgDevPrm.Device_mode,
		pcfg->CfgDevPrm.bFullscreen,
		pcfg->CfgDevPrm.bStereo
	};

    // Select a device. Ask for a hardware device that renders in a window.
	if (!SelectDevice (&m_pDeviceInfo, &dd))
		if (FAILED (hr = D3D7Enum_SelectDefaultDevice (&m_pDeviceInfo))) {
			LOGOUT_DDERR(hr);
			return false;
		}

    // Create a new CD3DFramework class. This class does all of our D3D
    // initialization and manages the common D3D objects.
	TRACENEW
    if (NULL == (m_pFramework = new CD3DFramework7())) {
        LOGOUT_DDERR (E_OUTOFMEMORY);
        return false;
    }

	// Output driver info to the SpaceXpanse.log file
	D3D7Enum_DeviceInfo *pDevices;
	DWORD nDevices;
	D3D7Enum_GetDevices (&pDevices, &nDevices);
	for (DWORD i = 0; i < nDevices; i++) {
		LOGOUT ("[%c] %s (%cW)",
			pDevices[i].guidDevice == m_pDeviceInfo->guidDevice ? 'x':' ',
			pDevices[i].strDesc, pDevices[i].bHardware ? 'H':'S');
	}

	// video tab instance
	vtab = new VideoTab (this);
	vtab->Init();

	// render parameter dialogs in extra tab
	spacexpanse::LaunchpadDialog *launchpad = g_pSpaceXpanse->Launchpad();
	spacexpanse::ExtraTab *tExtra = launchpad->GetExtraTab();
	HTREEITEM ht = launchpad->RegisterExtraParam (m_lpiGroup = new Extra_RenderOptions(tExtra), NULL);
	launchpad->RegisterExtraParam (m_lpiPlanetRenderOptions = new Extra_PlanetRenderOptions(tExtra), ht);

	return true;
}

// =======================================================================

void SpaceXpanseGraphics::clbkCleanup()
{
	spacexpanse::LaunchpadDialog* launchpad = g_pSpaceXpanse->Launchpad();
	launchpad->UnregisterExtraParam(m_lpiPlanetRenderOptions);
	delete m_lpiPlanetRenderOptions;
	m_lpiPlanetRenderOptions = nullptr;
	launchpad->UnregisterExtraParam(m_lpiGroup);
	delete m_lpiGroup;
	m_lpiGroup = nullptr;
}

// =======================================================================

void SpaceXpanseGraphics::clbkRenderScene ()
{
	//static bool bOutput2DData = true; // make configurable?
	HRESULT hr;

	// Check the cooperative level before rendering
	if (FAILED (hr = m_pDD->TestCooperativeLevel())) {
		switch (hr) {
		case DDERR_EXCLUSIVEMODEALREADYSET:
        case DDERR_NOEXCLUSIVEMODE:
            // Do nothing because some other app has exclusive mode
            return;
        case DDERR_WRONGMODE:
            // The display mode changed on us. Resize accordingly
            if (m_pDeviceInfo->bWindowed) {
                Change3DEnvironment();
				return;
			}
            break;
        }
        return;
    }

	// Set camera view transformation
	static D3DMATRIX mView = Identity;
	SetD3DRotation (mView, g_camera->GRot());
	D3DRECT rect = {(LONG)0,(LONG)0,(LONG)m_pFramework->GetRenderWidth(),(LONG)m_pFramework->GetRenderHeight()};

	// Render the scene
	if (bUseStereo && m_pDeviceInfo->bStereo && !m_pDeviceInfo->bWindowed) {

		// TODO: Translate view matrix for left eye
		m_pD3DDevice->SetTransform (D3DTRANSFORMSTATE_VIEW, &mView);
		m_pD3DDevice->SetRenderTarget (m_pddsRenderTargetLeft, 0);
		scene->Render (&rect);

		// TODO: Translate view matrix for right eye
		m_pD3DDevice->SetTransform (D3DTRANSFORMSTATE_VIEW, &mView);
		m_pD3DDevice->SetRenderTarget (m_pddsRenderTarget, 0);
		scene->Render (&rect);

	} else {
	
		m_pD3DDevice->SetTransform (D3DTRANSFORMSTATE_VIEW, &mView);
		scene->Render (&rect);

	}

	// 2D display overlay
	//if (bOutput2DData) spacexpanse->Output2DData();
}

// =======================================================================

bool SpaceXpanseGraphics::clbkDisplayFrame ()
{
	// Special rendering in the presence of popup windows
	if (RenderWithPopupWindows())
		return true;

    // Show the frame on the primary surface.
    HRESULT hr;
	if (FAILED (hr = m_pFramework->ShowFrame())) {
		if (hr != DDERR_SURFACELOST) return false;
		m_pFramework->RestoreSurfaces ();
		RestoreSurfaces ();
	}
	return true;
}

// =======================================================================

HWND SpaceXpanseGraphics::clbkCreateRenderWindow ()
{
	extern char *strWndClass;
	VIDEODATA *vd = GetVideoData();
	HWND hWnd;

	if (vd->fullscreen) {
		// remove window border to avoid problems with mouse coordinates
	    hWnd = CreateWindow (strWndClass, "",
			WS_POPUP | WS_EX_TOPMOST| WS_VISIBLE,
			CW_USEDEFAULT, CW_USEDEFAULT, 10, 10, 0, 0, SpaceXpanseInstance(), (LPVOID)this);
	} else {
	    hWnd = CreateWindow (strWndClass, "",
			WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
			CW_USEDEFAULT, CW_USEDEFAULT, vd->winw, vd->winh, 0, 0, SpaceXpanseInstance(), (LPVOID)this);
		//// Borderless:
		//hWnd = CreateWindow(strWndClass, "",
		//	WS_POPUP | WS_VISIBLE,
		//	CW_USEDEFAULT, CW_USEDEFAULT, vd->winw, vd->winh, 0, 0, SpaceXpanseInstance(), 0);
	}

	if (!hWnd) {
		LOGOUT ("ERROR: Could not create render window");
		MessageBox (NULL, "Could not create render window!", "Error",
			MB_OK | MB_ICONINFORMATION);
		return 0;
	}
	UpdateWindow (hWnd);

	spacexpanse->hRenderWnd = hWnd; // obsolete
	bNoVSync = vd->novsync;     // obsolete

	// Initialize the 3D environment for the app
	HRESULT hr;
    if (FAILED (hr = Init3DEnvironment ())) {
		LOGOUT("ERROR: Could not initialize 3D environment");
		spacexpanse->CloseSession ();
		LOGOUT_DDERR (hr);
        return 0;
	}

	ValidateRect (hWnd, NULL);
	// avoids white flash after splash screen

	InitOutputLoadStatus ();
	// prepare status output for splash screen

	return hWnd;
}

// =======================================================================

void SpaceXpanseGraphics::clbkPostCreation ()
{
	ExitOutputLoadStatus ();
}

// =======================================================================

bool SpaceXpanseGraphics::clbkSplashLoadMsg (const char *msg, int line)
{
	return OutputLoadStatus (msg, line);
}

// =======================================================================

void SpaceXpanseGraphics::clbkCloseSession (bool fastclose)
{
	if (!fastclose) {
		DestroyPatchDeviceObjects ();
		VObject::DestroyDeviceObjects ();
		PatchManager::DestroyDeviceObjects ();
		TileManager2Base::DestroyDeviceObjects ();
		VVessel::DestroyDeviceObjects ();
		if (scene) {
			delete scene;
			scene = NULL;
		}
	}
}

// =======================================================================

void SpaceXpanseGraphics::clbkDestroyRenderWindow (bool fastclose)
{
	GraphicsClient::clbkDestroyRenderWindow (fastclose);
	spacexpanse->bVisible = false;
	spacexpanse->bRunning = false;
	if (!fastclose)
		Exit3DEnvironment();
	else
		TileManager::Stop();
	spacexpanse->hRenderWnd = 0;
}

// =======================================================================

bool SpaceXpanseGraphics::clbkFullscreenMode () const
{
	return bFullscreen;
}

// =======================================================================

void SpaceXpanseGraphics::clbkGetViewportSize (DWORD *width, DWORD *height) const
{
	*width = viewW, *height = viewH;
}

// =======================================================================

bool SpaceXpanseGraphics::clbkGetRenderParam (DWORD prm, DWORD *value) const
{
	switch (prm) {
	case RP_COLOURDEPTH:
		*value = viewBPP;
		return true;
	case RP_ZBUFFERDEPTH:
		*value = GetFramework()->GetZBufferBitDepth();
		return true;
	case RP_STENCILDEPTH:
		*value = GetFramework()->GetStencilBitDepth();
		return true;
	case RP_MAXLIGHTS:
		*value = GetFramework()->GetMaxLights();
		return true;
	case RP_ISTLDEVICE:
		*value = GetFramework()->IsTLDevice();
		return true;
	case RP_REQUIRETEXPOW2:
		*value = GetFramework()->RequireTexPow2();
		return true;
	}
	return false;
}

// =======================================================================

int SpaceXpanseGraphics::clbkVisEvent (OBJHANDLE hObj, VISHANDLE vis, DWORD msg, DWORD_PTR context)
{
	VObject *vo = (VObject*)vis;
	vo->clbkEvent (msg, context);
	return 1;
}

// =======================================================================

MESHHANDLE SpaceXpanseGraphics::clbkGetMesh (VISHANDLE vis, UINT idx)
{
	if (!vis) return NULL; // sanity check
	return ((VVessel*)vis)->GetMesh (idx);
}

// =======================================================================

int SpaceXpanseGraphics::clbkGetMeshGroup (DEVMESHHANDLE hMesh, DWORD grpidx, GROUPREQUESTSPEC *grs)
{
	return ((Mesh*)hMesh)->GetGroup (grpidx, grs);
}

// =======================================================================

int SpaceXpanseGraphics::clbkEditMeshGroup (DEVMESHHANDLE hMesh, DWORD grpidx, GROUPEDITSPEC *ges)
{
	return ((Mesh*)hMesh)->EditGroup (grpidx, ges);
}

// =======================================================================

void SpaceXpanseGraphics::clbkUpdate (bool running)
{
	extern bool g_bForceUpdate; // (temporary?)
	scene->Update (g_psys, &g_camera, 1, running, g_bForceUpdate);
}

// =======================================================================

void SpaceXpanseGraphics::clbkTimeJump (double simt, double simdt, double mjd)
{
	scene->Timejump (g_psys, &g_camera, 1, spacexpanse->bRunning);
}

// ==============================================================

void SpaceXpanseGraphics::clbkPreOpenPopup ()
{
	if (clipper) m_pDD->FlipToGDISurface();
}

// =======================================================================

void SpaceXpanseGraphics::clbkNewVessel (OBJHANDLE hVessel)
{
	scene->AddBody ((Body*)hVessel, &g_camera, 1);
}

// =======================================================================

void SpaceXpanseGraphics::clbkDeleteVessel (OBJHANDLE hVessel)
{
	scene->DelVisual ((Body*)hVessel);
}

// =======================================================================

void SpaceXpanseGraphics::clbkVesselJump (OBJHANDLE hVessel)
{
	scene->Update (g_psys, &g_camera, 1, false, false);
}

// =======================================================================

void SpaceXpanseGraphics::clbkRender2DPanel (SURFHANDLE *hSurf, MESHHANDLE hMesh, MATRIX3 *T, bool transparent)
{
	static DWORD nHVTX = 1;
	static struct HVTX {
		float x, y, z;
		float rhw;
		float tu, tv;
	} *hvtx = new HVTX[1];

	DWORD vtxFmt = D3DFVF_XYZRHW | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0);
	DWORD dAlpha;
	m_pD3DDevice->GetRenderState (D3DRENDERSTATE_ALPHABLENDENABLE, &dAlpha);
	m_pD3DDevice->SetRenderState (D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_ADDRESS, D3DTADDRESS_CLAMP);
	if (transparent)
		m_pD3DDevice->SetRenderState (D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);
	float rhw = 1;
	DWORD i, j, nvtx, ngrp = oapiMeshGroupCount (hMesh);
	SURFHANDLE surf = (SURFHANDLE)-1, newsurf = 0;

	float scalex = (float)(T->m11),  dx = (float)(T->m13);
	float scaley = (float)(T->m22),  dy = (float)(T->m23);

	for (i = 0; i < ngrp; i++) {
		MESHGROUP *grp = oapiMeshGroup (hMesh, i);
		if (grp->UsrFlag & 0x2) continue; // skip this group

		if (grp->UsrFlag & 0x8) { // additive blend
			m_pD3DDevice->SetRenderState (D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
			m_pD3DDevice->SetRenderState (D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);
		}

		if (grp->TexIdx == SPEC_DEFAULT) {
			newsurf = 0;
		} else if (grp->TexIdx == SPEC_INHERIT) {
			// nothing to do
		} else if (grp->TexIdx >= TEXIDX_MFD0) {
			int mfdidx = grp->TexIdx-TEXIDX_MFD0;
			newsurf = GetMFDSurface (mfdidx);
			if (!newsurf) continue;
		} else if (hSurf) {
			newsurf = hSurf[grp->TexIdx];
		} else {
			newsurf = oapiGetTextureHandle (hMesh, grp->TexIdx+1);
		}
		if (newsurf != surf) {
			m_pD3DDevice->SetTexture (0, (LPDIRECTDRAWSURFACE7)(surf = newsurf));
		}

		nvtx = grp->nVtx;
		if (nvtx > nHVTX) { // increase buffer size
			delete []hvtx;
			hvtx = new HVTX[nHVTX = nvtx];
			for (j = 0; j < nvtx; j++) {
				hvtx[j].z = 0;
				hvtx[j].rhw = rhw;
			}
		}
		for (j = 0; j < nvtx; j++) {
			HVTX *tgtvtx = hvtx+j;
			NTVERTEX *srcvtx = grp->Vtx+j;
			tgtvtx->x = srcvtx->x*scalex + dx;
			tgtvtx->y = srcvtx->y*scaley + dy;
			tgtvtx->tu = srcvtx->tu;
			tgtvtx->tv = srcvtx->tv;
		}
		m_pD3DDevice->DrawIndexedPrimitive (D3DPT_TRIANGLELIST, vtxFmt, hvtx, nvtx, grp->Idx, grp->nIdx, 0);

		if (grp->UsrFlag & 0x8) { // reset additive
			m_pD3DDevice->SetRenderState (D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
			m_pD3DDevice->SetRenderState (D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
	}
	
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_ADDRESS, D3DTADDRESS_WRAP);
	if (transparent)
		m_pD3DDevice->SetRenderState (D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
	if (dAlpha != TRUE)
		m_pD3DDevice->SetRenderState (D3DRENDERSTATE_ALPHABLENDENABLE, dAlpha);
}

// =======================================================================

void SpaceXpanseGraphics::clbkRender2DPanel (SURFHANDLE *hSurf, MESHHANDLE hMesh, MATRIX3 *T, float alpha, bool additive)
{
	bool reset = false;
	DWORD alphaop, alphaarg2, tfactor;
	if (alpha < 1.0f) {
		m_pD3DDevice->GetTextureStageState (0, D3DTSS_ALPHAOP, &alphaop);
		m_pD3DDevice->GetTextureStageState (0, D3DTSS_ALPHAARG2, &alphaarg2);
		m_pD3DDevice->GetRenderState (D3DRENDERSTATE_TEXTUREFACTOR, &tfactor);
		m_pD3DDevice->SetTextureStageState (0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		m_pD3DDevice->SetTextureStageState (0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
		m_pD3DDevice->SetRenderState (D3DRENDERSTATE_TEXTUREFACTOR, D3DRGBA(1,1,1,alpha));
		reset = true;
	}
	clbkRender2DPanel (hSurf, hMesh, T, additive);
	if (reset) {
		m_pD3DDevice->SetTextureStageState (0, D3DTSS_ALPHAOP, alphaop);
		m_pD3DDevice->SetTextureStageState (0, D3DTSS_ALPHAARG2, alphaarg2);
		m_pD3DDevice->SetRenderState (D3DRENDERSTATE_TEXTUREFACTOR, tfactor);
	}
}

// =======================================================================

void SpaceXpanseGraphics::clbkSetCamera (double aspect, double tan_ap, double nearplane, double farplane)
{
	static D3DMATRIX mProj;

	if (m_pD3DDevice) {
		ZeroMemory (&mProj, sizeof (D3DMATRIX));
		mProj._11 = (FLOAT)(aspect / tan_ap);
		mProj._22 = (FLOAT)(1.0    / tan_ap);
		mProj._43 = (mProj._33 = (float)(farplane / (farplane-nearplane))) * (float)(-nearplane);
		mProj._34 = 1.0f;

		// register new projection matrix with device
		m_pD3DDevice->SetTransform (D3DTRANSFORMSTATE_PROJECTION, &mProj);
	}
}

// =======================================================================

HRESULT SpaceXpanseGraphics::Init3DEnvironment ()
{
	HRESULT hr;
	DWORD dwFrameworkFlags = 0L;
	dwFrameworkFlags |= (!m_pDeviceInfo->bWindowed ? D3DFW_FULLSCREEN : 0L);
	dwFrameworkFlags |= ( m_pDeviceInfo->bStereo   ? D3DFW_STEREO     : 0L);
	dwFrameworkFlags |= ( bUseZBuffer              ? D3DFW_ZBUFFER    : 0L);
	dwFrameworkFlags |= ( bNoVSync                 ? D3DFW_NOVSYNC    : 0L);
	dwFrameworkFlags |= ( spacexpanse->Cfg()->CfgDevPrm.bPageflip ? D3DFW_PAGEFLIP : 0L);
	dwFrameworkFlags |= ( spacexpanse->Cfg()->CfgDevPrm.bTryStencil ? D3DFW_TRYSTENCIL : 0L);

	dwFrameworkFlags |= D3DFW_NO_FPUSETUP; // SpaceXpanse needs double-precision FPU

	if (SUCCEEDED (hr = m_pFramework->Initialize (spacexpanse->hRenderWnd,
		m_pDeviceInfo->pDriverGUID,
		m_pDeviceInfo->pDeviceGUID,
	   &m_pDeviceInfo->ddsdFullscreenMode,
		dwFrameworkFlags))) {

		m_pDD        = m_pFramework->GetDirectDraw();
        m_pD3D       = m_pFramework->GetDirect3D();
        m_pD3DDevice = m_pFramework->GetD3DDevice();

		m_pddsRenderTarget        = m_pFramework->GetRenderSurface();
        m_pddsRenderTargetLeft    = m_pFramework->GetRenderSurfaceLeft();
        m_ddsdRenderTarget.dwSize = sizeof(m_ddsdRenderTarget);
        m_pddsRenderTarget->GetSurfaceDesc (&m_ddsdRenderTarget);

		// Get dimensions of the render surface
		viewW   = m_ddsdRenderTarget.dwWidth;
		viewH   = m_ddsdRenderTarget.dwHeight;
		viewBPP = m_ddsdRenderTarget.ddpfPixelFormat.dwRGBBitCount;

		// Get additional parameters
		bFullscreen = (m_pFramework->IsFullscreen() ? true:false);

		ShowDefaultSplash();
		m_pddsRenderTarget->BltFast (0, 0, m_pFramework->GetFrontBuffer(), NULL, DDBLTFAST_WAIT);

		// Output some render parameters to the log
		LogRenderParams();

		// Create texture manager
		g_texmanager2 = new TextureManager2 (m_pD3DDevice); TRACENEW

		// Create clipper object
		if (bFullscreen) {
			if (m_pDD->CreateClipper (0, &clipper, NULL) == DD_OK)
				clipper->SetHWnd (0, spacexpanse->hRenderWnd);
		}

		// Create dialog manager instance
		//dlgmgr = new DialogManager;
		//dlgmgr->Init (spacexpanse->hRenderWnd, this);

		// Create scene instance
		scene = new Scene (this);

		// Let the app run its startup code which creates the 3d scene.
        if( SUCCEEDED (hr = spacexpanse->InitDeviceObjects())) {
			return S_OK;
		} else {
			LOGOUT_ERR("Init device objects failed");
            spacexpanse->DeleteDeviceObjects();
            m_pFramework->DestroyObjects();
        }
	}

	// If you get here, the first initialization pass failed. If
	// that was with a hardware device, suggest using a software device
	if (m_pDeviceInfo->bHardware) {
		MessageBox (NULL, "Initialising 3D environment using a hardware device failed.\nTry again with a software device.",
			"Error", MB_OK | MB_ICONERROR);
	}
	return hr;
}

// =======================================================================

HRESULT SpaceXpanseGraphics::Change3DEnvironment ()
{
    HRESULT hr;
    static BOOL  bOldWindowedState = TRUE;
    static DWORD dwSavedStyle;
    static RECT  rcSaved;

    // Release all scene objects that will be re-created for the new device
    spacexpanse->DeleteDeviceObjects ();

    // Release framework objects, so a new device can be created
    if (FAILED (hr = m_pFramework->DestroyObjects())) {
		LOGOUT_DDERR (hr);
        SendMessage (spacexpanse->hRenderWnd, WM_CLOSE, 0, 0);
        return hr;
    }

    // Check if going from fullscreen to windowed mode, or vice versa.
    if (bOldWindowedState != m_pDeviceInfo->bWindowed) {
        if (m_pDeviceInfo->bWindowed) {
            // Coming from fullscreen mode, so restore window properties
            SetWindowLongPtr (spacexpanse->hRenderWnd, GWL_STYLE, dwSavedStyle);
            SetWindowPos (spacexpanse->hRenderWnd, HWND_NOTOPMOST, rcSaved.left, rcSaved.top,
                         (rcSaved.right - rcSaved.left), 
                         (rcSaved.bottom - rcSaved.top), SWP_SHOWWINDOW);
        } else {
            // Going to fullscreen mode, save/set window properties as needed
            dwSavedStyle = GetWindowLongPtr (spacexpanse->hRenderWnd, GWL_STYLE);
            GetWindowRect (spacexpanse->hRenderWnd, &rcSaved);
            SetWindowLongPtr (spacexpanse->hRenderWnd, GWL_STYLE, WS_POPUP|WS_SYSMENU|WS_VISIBLE);
        }
        bOldWindowedState = m_pDeviceInfo->bWindowed;
    }

    // Inform the framework class of the driver change. It will internally
    // re-create valid surfaces, a d3ddevice, etc.
    if (FAILED (hr = Init3DEnvironment())) {
		LOGOUT_DDERR (hr);
        SendMessage (spacexpanse->hRenderWnd, WM_CLOSE, 0, 0);
        return hr;
    }

    return S_OK;
}

// =======================================================================

void SpaceXpanseGraphics::Exit3DEnvironment ()
{
	//g_psys->DestroyDeviceObjects();
	TileManager::DestroyDeviceObjects ();
	CSphereManager::DestroyDeviceObjects ();
	if (clipper) {
		clipper->Release();
		clipper = NULL;
	}
	if (g_texmanager2) {
		delete g_texmanager2;
		g_texmanager2 = NULL;
	}
	if (m_pFramework) {
		bool showrefwarning = (spacexpanse->Cfg()->CfgDebugPrm.ShutdownMode == 0);
		spacexpanse->DeleteDeviceObjects ();
		if (FAILED (m_pFramework->DestroyObjects(showrefwarning)))
			if (showrefwarning)
				LOGOUT_ERR("Destroy framework objects failed");
	}
}

// =======================================================================
// Restores any previously lost surfaces. Must do this for all surfaces
// (including textures) that the app created.
// Note this may not be necessary since the framework restores
// all surfaces. But surface bitmaps may be rebuild here

HRESULT SpaceXpanseGraphics::RestoreSurfaces ()
{
	// not implemented
	return S_OK;
}

// =======================================================================

bool SpaceXpanseGraphics::RenderWithPopupWindows ()
{
	if (!bFullscreen) return false; // no special treatment required in windowed mode

	DWORD nWnd;
	const HWND *hPopupWnd;
	nWnd = GetPopupList (&hPopupWnd);
	if (!nWnd) return false; // nothing to do

	LPDIRECTDRAWSURFACE7 front = m_pFramework->GetFrontBuffer();
	LPDIRECTDRAWSURFACE7 back  = m_pFramework->GetBackBuffer();

	if (clipper) {
		front->SetClipper (clipper);
		front->Blt (NULL, back, NULL, DDBLT_WAIT, NULL);
		return true;
	} else {
		RECT rc;
		int x, y, cx, cy;
		DWORD i;
		HDC	hdcScreen, hdcBackBuffer;
		HRGN hrgn;
		hdcScreen = GetDC(NULL);
		back->GetDC (&hdcBackBuffer);
		for (i = 0; i < nWnd; i++) {
			HWND hDlg = hPopupWnd[i];
			GetWindowRect (hDlg, &rc);
			x  = rc.left;
			y  = rc.top;
			cx = rc.right - rc.left;
			cy = rc.bottom - rc.top;

			// If window has a complex region associated with it, be sure to include it in the draw
			hrgn = CreateRectRgn(0, 0, 0, 0);
			if (GetWindowRgn(hDlg, hrgn) == COMPLEXREGION) {
				OffsetRgn(hrgn, rc.left, rc.top);
				SelectClipRgn(hdcBackBuffer, hrgn);
			}

			BitBlt (hdcBackBuffer, x, y, cx, cy, hdcScreen, x, y, SRCCOPY);

			// Remove clipping region and clean up
			SelectClipRgn (hdcBackBuffer, NULL);
			DeleteObject(hrgn);
		}
		back->ReleaseDC (hdcBackBuffer);
		ReleaseDC (NULL, hdcScreen);
		return false;
		// we return false because the page still needs to be flipped
	}
}

// =======================================================================

static const DWORD LOADHEADERCOL = 0xB0B0B0; //0xFFFFFF
static const DWORD LOADSTATUSCOL = 0xC08080; //0xFFD0D0

void SpaceXpanseGraphics::InitOutputLoadStatus ()
{
#ifdef UNDEF
	HDC hDC;
	HFONT hFont = 0;
	TEXTMETRIC tm;
	DWORD sw = viewW, sh = viewH;

	m_pFramework->GetBackBuffer()->GetDC (&hDC);
	if (splashFont)
		hFont = (HFONT)SelectObject (hDC, ((GDIFont*)splashFont)->GetGDIFont());
	GetTextMetrics (hDC, &tm);
	DWORD fontsize = tm.tmHeight;
	lstatus.x = sw/2;
	lstatus.y = sh-fontsize*2;
	lstatus.h = fontsize*2;
	lstatus.w = sw-fontsize*40-lstatus.x;

	HBITMAP hBmp = CreateCompatibleBitmap (hDC, lstatus.w+15, lstatus.h+lstatus.h/2+10);
	lstatus.hBkg = CreateCompatibleBitmap (hDC, lstatus.w, lstatus.h);
	lstatus.bkgDC = CreateCompatibleDC (hDC);
	SelectObject (lstatus.bkgDC, hBmp);
	SelectObject (lstatus.bkgDC, GetStockObject(GRAY_BRUSH));
	Rectangle (lstatus.bkgDC, 0, 0, lstatus.w+15, lstatus.h+lstatus.h/2+10);
	SelectObject (lstatus.bkgDC, lstatus.hBkg);
	DeleteObject (hBmp);
	BitBlt (lstatus.bkgDC, 0, 0, lstatus.w, lstatus.h, hDC, lstatus.x, lstatus.y, SRCCOPY);
	SelectObject (hDC, GetStockObject (NULL_BRUSH));
	SetTextColor (hDC, LOADHEADERCOL);
	SetBkMode (hDC, TRANSPARENT);
	SetTextAlign (hDC, TA_RIGHT);
	//TextOut (hDC, lstatus.x-fontsize, lstatus.y, "Loading", 7);
	if (hFont) SelectObject (hDC, hFont);
	m_pFramework->GetBackBuffer()->ReleaseDC (hDC);
	clbkBlt (m_pFramework->GetFrontBuffer(), 0, 0, m_pFramework->GetBackBuffer());
#endif
}

// =======================================================================

void SpaceXpanseGraphics::ExitOutputLoadStatus ()
{
#ifdef UNDEF
	DeleteObject (lstatus.hBkg);
	DeleteDC (lstatus.bkgDC);
	lstatus.bkgDC = 0;
#endif
}

// =======================================================================

bool SpaceXpanseGraphics::OutputLoadStatus (const char *msg, int line)
{
	return false; // skip this for now

	static char linebuf[2][70] = {"", ""};

	if (!lstatus.bkgDC) return false;

	HDC hDC;
	int i;
	strncpy (linebuf[line], msg, 64); linebuf[line][63] = '\0';
	m_pFramework->GetBackBuffer()->GetDC (&hDC);
	SetTextColor (hDC, LOADSTATUSCOL);
	SetBkMode (hDC, TRANSPARENT);
	HFONT hFont = 0;
	if (splashFont)
		hFont = (HFONT)SelectObject (hDC, ((GDIFont*)splashFont)->GetGDIFont());
	BitBlt (hDC, lstatus.x, lstatus.y, lstatus.w, lstatus.h, lstatus.bkgDC, 0, 0, SRCCOPY);
	for (i = 0; i < 2; i++) {
		if (linebuf[i][0]) 
			TextOut (hDC, lstatus.x, lstatus.y+i*lstatus.h/2, linebuf[i], strlen (linebuf[i]));
	}
	if (hFont) SelectObject (hDC, hFont);
	m_pFramework->GetBackBuffer()->ReleaseDC (hDC);
	clbkDisplayFrame ();
	return true;
}

// =======================================================================

LRESULT SpaceXpanseGraphics::RenderWndProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {

    case WM_PAINT:
        // Handle paint messages when the app is not ready
        if (m_pFramework && !spacexpanse->bRunning) {
			if (m_pDeviceInfo->bWindowed)
				m_pFramework->ShowFrame();
            //else
            //    m_pFramework->FlipToGDISurface (TRUE);
        }
        break;

	case WM_MOVE:
		// If in windowed mode, move the Framework's window
		if (m_pFramework && m_pDeviceInfo->bWindowed)
			m_pFramework->Move ((SHORT)LOWORD(lParam), (SHORT)HIWORD(lParam));
		break;

	case WM_SIZE:
		// Check to see if we are losing our window...
		spacexpanse->bVisible = (wParam != SIZE_MAXHIDE && wParam != SIZE_MINIMIZED);
#ifdef UNDEF // have to think about this
		// A new window size will require a new backbuffer
		// size, so the 3D structures must be changed accordingly.
		if (spacexpanse->m_bActive && spacexpanse->m_bReady && m_pDeviceInfo->bWindowed) {
			spacexpanse->m_bReady = FALSE;
			if (FAILED (hr = Change3DEnvironment())) return 0;
			spacexpanse->m_bReady = TRUE;
        }
#endif
		break;

#ifdef UNDEF // we allow a cursor in fullscreen mode now!
	case WM_SETCURSOR:
		// Prevent a cursor in fullscreen mode
		if (spacexpanse->m_bVisible && !m_pDeviceInfo->bWindowed) {
			SetCursor(NULL);
			return 1;
		}
		break;
#endif

	case WM_SYSCOMMAND:
		switch (wParam) {
		case SC_KEYMENU:
			// trap Alt system keys
			return 1;
		case SC_MOVE:
		case SC_SIZE:
		case SC_MAXIMIZE:
		case SC_MONITORPOWER:
			// Prevent moving/sizing and power loss in fullscreen mode
			if (FALSE == m_pDeviceInfo->bWindowed)
				return 1;
			break;
		}
		break;

	case WM_SYSKEYUP:
		if (FALSE == m_pDeviceInfo->bWindowed)
			return 0;  // trap Alt-key

	}
	return spacexpanse->MsgProc (hWnd, uMsg, wParam, lParam);
}

// =======================================================================

INT_PTR SpaceXpanseGraphics::LaunchpadVideoWndProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (vtab) return vtab->WndProc (hWnd, uMsg, wParam, lParam);
	else return GraphicsClient::LaunchpadVideoWndProc (hWnd, uMsg, wParam, lParam);
}

// =======================================================================
// Particle stream functions
// =======================================================================

oapi::ParticleStream *SpaceXpanseGraphics::clbkCreateParticleStream (PARTICLESTREAMSPEC *pss)
{
	return NULL;
}

// =======================================================================

oapi::ParticleStream *SpaceXpanseGraphics::clbkCreateExhaustStream (PARTICLESTREAMSPEC *pss,
	OBJHANDLE hVessel, const double *lvl, const VECTOR3 *ref, const VECTOR3 *dir)
{
	ExhaustStream *es = new ExhaustStream (this, hVessel, lvl, ref, dir, pss); TRACENEW
	GetScene()->AddParticleStream (es);
	return es;
}

// =======================================================================

oapi::ParticleStream *SpaceXpanseGraphics::clbkCreateExhaustStream (PARTICLESTREAMSPEC *pss,
	OBJHANDLE hVessel, const double *lvl, const VECTOR3 &ref, const VECTOR3 &dir)
{
	ExhaustStream *es = new ExhaustStream (this, hVessel, lvl, ref, dir, pss); TRACENEW
	GetScene()->AddParticleStream (es);
	return es;
}

// ======================================================================

oapi::ParticleStream *SpaceXpanseGraphics::clbkCreateReentryStream (PARTICLESTREAMSPEC *pss,
	OBJHANDLE hVessel)
{
	ReentryStream *rs = new ReentryStream (this, hVessel, pss); TRACENEW
	GetScene()->AddParticleStream (rs);
	return rs;
}

// ======================================================================

bool SpaceXpanseGraphics::clbkParticleStreamExists (const oapi::ParticleStream *ps)
{
	Scene *scn = GetScene();
	return (scn != NULL && scn->ParticleStreamExists (ps));
}

// =======================================================================
// Texture functions
// =======================================================================

SURFHANDLE SpaceXpanseGraphics::clbkLoadTexture (const char *fname, DWORD flags)
{
	LPDIRECTDRAWSURFACE7 tex =  NULL;
	if (flags & 8) // managed texture
		tex = g_texmanager->AcquireTexture (fname, (flags & 2) != 0);
	else {
		FILE *f = spacexpanse->OpenTextureFile (fname, "");
		if (f) {
			g_texmanager2->ReadTexture (f, &tex, flags);
			fclose (f);
		}
	}
	return (SURFHANDLE)tex;
}

// ==============================================================

void SpaceXpanseGraphics::clbkReleaseTexture (SURFHANDLE hTex)
{
	LPDIRECTDRAWSURFACE7 tex = (LPDIRECTDRAWSURFACE7)hTex;
	tex->Release();
}

// ==============================================================

bool SpaceXpanseGraphics::clbkSetMeshTexture (DEVMESHHANDLE hMesh, DWORD texidx, SURFHANDLE tex)
{
	Mesh *mesh = (Mesh*)hMesh;
	return mesh->SetTexture (texidx-1, tex, false);
}

// ==============================================================

int SpaceXpanseGraphics::clbkSetMeshMaterial (DEVMESHHANDLE hMesh, DWORD matidx, const MATERIAL *mat)
{
	Mesh *mesh = (Mesh*)hMesh;
	DWORD nmat = mesh->nMaterial();
	if (matidx >= nmat) return 4; // "index out of range"
	D3DMATERIAL7 *meshmat = mesh->GetMaterial (matidx);
	memcpy (meshmat, mat, sizeof(D3DMATERIAL7)); // relies on D3DMATERIAL7 and MATERIAL to be equivalent
	return 0;
}

// ==============================================================

int SpaceXpanseGraphics::clbkMeshMaterial (DEVMESHHANDLE hMesh, DWORD matidx, MATERIAL *mat)
{
	Mesh *mesh = (Mesh*)hMesh;
	DWORD nmat = mesh->nMaterial();
	if (matidx >= nmat) return 4; // "index out of range"
	D3DMATERIAL7 *meshmat = mesh->GetMaterial (matidx);
	memcpy (mat, meshmat, sizeof(D3DMATERIAL7)); // relies on D3DMATERIAL7 and MATERIAL to be equivalent
	return 0;
}

int SpaceXpanseGraphics::clbkSetMaterialEx(DEVMESHHANDLE hMesh, DWORD matidx, MatProp mat, const oapi::FVECTOR4* in)
{
	Mesh* mesh = (Mesh*)hMesh;
	DWORD nmat = mesh->nMaterial();
	if (matidx >= nmat) return 4; // "index out of range"
	D3DMATERIAL7* meshmat = mesh->GetMaterial(matidx);
	switch (mat) {
	case MatProp::Diffuse:
		meshmat->diffuse = *((D3DCOLORVALUE*)in);
		return 0;
	case MatProp::Ambient:
		meshmat->ambient = *((D3DCOLORVALUE*)in);
		return 0;
	case MatProp::Specular:
		meshmat->specular = *((D3DCOLORVALUE*)in);
		meshmat->specular.a = 0.0f;
		meshmat->power = ((D3DCOLORVALUE*)in)->a;
		return 0;
	case MatProp::Light:
		meshmat->emissive = *((D3DCOLORVALUE*)in);
		return 0;
	}
	return 5;
}

// ==============================================================

int SpaceXpanseGraphics::clbkMeshMaterialEx(DEVMESHHANDLE hMesh, DWORD matidx, MatProp mat, oapi::FVECTOR4* out)
{
	Mesh* mesh = (Mesh*)hMesh;
	DWORD nmat = mesh->nMaterial();
	if (matidx >= nmat) return 4; // "index out of range"
	D3DMATERIAL7* meshmat = mesh->GetMaterial(matidx);

	switch (mat) {
	case MatProp::Diffuse:
		*((D3DCOLORVALUE*)out) = meshmat->diffuse;
		return 0;
	case MatProp::Ambient:
		*((D3DCOLORVALUE*)out) = meshmat->ambient;
		return 0;
	case MatProp::Specular:
		*((D3DCOLORVALUE*)out) = meshmat->specular;
		((D3DCOLORVALUE*)out)->a = meshmat->power;
		return 0;
	case MatProp::Light:
		*((D3DCOLORVALUE*)out) = meshmat->emissive;
		return 0;
	}
	return 5;
}

// ==============================================================

bool SpaceXpanseGraphics::clbkSetMeshProperty (DEVMESHHANDLE hMesh, DWORD property, DWORD value)
{
	Mesh *mesh = (Mesh*)hMesh;
	switch (property) {
	case MESHPROPERTY_MODULATEMATALPHA:
		mesh->EnableMatAlpha (value != 0);
		return true;
	}
	return false;
}

// =======================================================================
// Surface functions
// =======================================================================

SURFHANDLE SpaceXpanseGraphics::clbkCreateSurface (DWORD w, DWORD h, SURFHANDLE hTemplate)
{
	LPDIRECTDRAWSURFACE7 surf;
	DDSURFACEDESC2 ddsd;
    ZeroMemory (&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH; 
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    ddsd.dwWidth  = w;
	ddsd.dwHeight = h;
	if (hTemplate) { // use pixel format information from template surface
		ddsd.ddpfPixelFormat.dwSize = sizeof (DDPIXELFORMAT);
		ddsd.ddpfPixelFormat.dwFlags = 0;
		((LPDIRECTDRAWSURFACE7)hTemplate)->GetPixelFormat (&ddsd.ddpfPixelFormat);
		ddsd.dwFlags |= DDSD_PIXELFORMAT;
	}
	if (m_pDD->CreateSurface (&ddsd, &surf, NULL) != DD_OK)
		return NULL;
	return (SURFHANDLE)surf;
}

SURFHANDLE SpaceXpanseGraphics::clbkCreateSurfaceEx (DWORD w, DWORD h, DWORD attrib)
{
	HRESULT hr;
	LPDIRECTDRAWSURFACE7 surf;
	DDSURFACEDESC2 ddsd;
    ZeroMemory (&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

    ddsd.dwWidth  = w;
	ddsd.dwHeight = h;

	// by default, take pixel format from render surface (should make it compatible
	ddsd.ddpfPixelFormat.dwSize = sizeof (DDPIXELFORMAT);
	ddsd.ddpfPixelFormat.dwFlags = 0;
	GetFramework()->GetBackBuffer()->GetPixelFormat (&ddsd.ddpfPixelFormat);

    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT; 

	if (attrib & OAPISURFACE_TEXTURE)
		ddsd.ddsCaps.dwCaps |= DDSCAPS_TEXTURE;
	else
		ddsd.ddsCaps.dwCaps |= DDSCAPS_OFFSCREENPLAIN;

	if (attrib & OAPISURFACE_SYSMEM)
		ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
	else
		ddsd.ddsCaps.dwCaps |= DDSCAPS_VIDEOMEMORY;

	if (attrib & OAPISURFACE_NOALPHA || attrib & OAPISURFACE_GDI || attrib & OAPISURFACE_SKETCHPAD)
		ddsd.ddpfPixelFormat.dwFlags &= ~DDPF_ALPHAPIXELS; // turn off alpha data (note: this should be the default for the backbuffer)
	else if (attrib & OAPISURFACE_ALPHA)
		ddsd.ddpfPixelFormat.dwFlags |= DDPF_ALPHAPIXELS; // turn on alpha data

	if (attrib & OAPISURFACE_RENDER3D)
		ddsd.ddsCaps.dwCaps |= DDSCAPS_3DDEVICE;

	if ((hr = m_pDD->CreateSurface (&ddsd, &surf, NULL)) != DD_OK) {
		LOGOUT_DDERR_ONCE (hr);
		return NULL;
	}

	//DDSURFACEDESC2 desc;
	//memset (&desc, 0, sizeof(DDSURFACEDESC2));
	//desc.dwSize = sizeof(DDSURFACEDESC2);
	//surf->GetSurfaceDesc (&desc);

	return (SURFHANDLE)surf;
}

SURFHANDLE SpaceXpanseGraphics::clbkCreateTexture (DWORD w, DWORD h)
{
	LPDIRECTDRAWSURFACE7 surf;
	DDSURFACEDESC2 ddsd;
    ZeroMemory (&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT | DDSD_CAPS /*| DDSD_CKSRCBLT*/;
	ddsd.dwWidth = w;
	ddsd.dwHeight = h;
	ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE;
	ddsd.ddpfPixelFormat.dwSize = sizeof (DDPIXELFORMAT);
	ddsd.ddpfPixelFormat.dwFlags = 0;
	GetFramework()->GetBackBuffer()->GetPixelFormat (&ddsd.ddpfPixelFormat);
	// take pixel format from render surface (should make it compatible
	//ddsd.ddpfPixelFormat.dwFlags &= ~DDPF_ALPHAPIXELS; // turn off alpha data

	m_pDD->CreateSurface (&ddsd, &surf, NULL);
	return surf;
}

void SpaceXpanseGraphics::clbkIncrSurfaceRef (SURFHANDLE surf)
{
	ULONG count;
	count = ((LPDIRECTDRAWSURFACE7)surf)->AddRef();
}

bool SpaceXpanseGraphics::clbkReleaseSurface (SURFHANDLE surf)
{
	if (surf) {
		ULONG count;
		count = ((LPDIRECTDRAWSURFACE7)surf)->Release();
		return true;
	} else
		return false;
}

bool SpaceXpanseGraphics::clbkGetSurfaceSize (SURFHANDLE surf, DWORD *w, DWORD *h)
{
    DDSURFACEDESC2          ddsd;
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
    ((LPDIRECTDRAWSURFACE7)surf)->GetSurfaceDesc(&ddsd);
	*w = ddsd.dwWidth;
	*h = ddsd.dwHeight;
	return true;
}

bool SpaceXpanseGraphics::clbkSetSurfaceColourKey (SURFHANDLE surf, DWORD ckey)
{
	DDCOLORKEY ck = {ckey,ckey};
	((LPDIRECTDRAWSURFACE7)surf)->SetColorKey (DDCKEY_SRCBLT, &ck);
	return true;
}

DWORD SpaceXpanseGraphics::clbkGetDeviceColour (BYTE r, BYTE g, BYTE b)
{
	if (viewBPP >= 24) return ((DWORD)r << 16) + ((DWORD)g << 8) + (DWORD)b;
	else               return ((((DWORD)r*319)/2559) << 11) + ((((DWORD)g*639)/2559) << 5) + (((DWORD)b*319)/2559);
}

// =======================================================================
// Blitting functions
// =======================================================================

bool SpaceXpanseGraphics::clbkBlt (SURFHANDLE tgt, DWORD tgtx, DWORD tgty, SURFHANDLE src, DWORD flag) const
{
	LPDIRECTDRAWSURFACE7 ps_tgt = (tgt ? (LPDIRECTDRAWSURFACE7)tgt : m_pddsRenderTarget);
	LPDIRECTDRAWSURFACE7 ps_src = (LPDIRECTDRAWSURFACE7)src;
	DWORD bltflag = DDBLTFAST_WAIT;
	if (flag & BLT_SRCCOLORKEY) bltflag |= DDBLTFAST_SRCCOLORKEY;
	if (flag & BLT_TGTCOLORKEY) bltflag |= DDBLTFAST_DESTCOLORKEY;
	HRESULT hr;
	if ((hr = ps_tgt->BltFast (tgtx, tgty, ps_src, NULL, bltflag)) != S_OK) {
		DDSURFACEDESC2 ddsd;
		memset (&ddsd, 0, sizeof(DDSURFACEDESC2));
		ddsd.dwSize = sizeof(DDSURFACEDESC2);
		ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
		ps_src->GetSurfaceDesc (&ddsd);
		bltflag = DDBLT_WAIT;
		if (flag & BLT_SRCCOLORKEY) bltflag |= DDBLT_KEYSRC;
		if (flag & BLT_TGTCOLORKEY) bltflag |= DDBLT_KEYDEST;
		RECT dstrct = {(LONG)tgtx, (LONG)tgty, (LONG)(tgtx+ddsd.dwWidth), (LONG)(tgty+ddsd.dwHeight)};
		hr = ps_tgt->Blt (&dstrct, ps_src, NULL, bltflag, NULL);
		if (hr != S_OK)
			LOGOUT_DDERR (hr);
	}
	return (hr == S_OK);
}

bool SpaceXpanseGraphics::clbkBlt (SURFHANDLE tgt, DWORD tgtx, DWORD tgty, SURFHANDLE src, DWORD srcx, DWORD srcy, DWORD w, DWORD h, DWORD flag) const
{
	RECT srcr = {(LONG)srcx, (LONG)srcy, (LONG)(srcx+w), (LONG)(srcy+h)};
	LPDIRECTDRAWSURFACE7 ps_tgt = (tgt ? (LPDIRECTDRAWSURFACE7)tgt : m_pddsRenderTarget);
	LPDIRECTDRAWSURFACE7 ps_src = (LPDIRECTDRAWSURFACE7)src;
	DWORD bltflag = DDBLTFAST_WAIT;
	if (flag & BLT_SRCCOLORKEY) bltflag |= DDBLTFAST_SRCCOLORKEY;
	if (flag & BLT_TGTCOLORKEY) bltflag |= DDBLTFAST_DESTCOLORKEY;
	HRESULT hr;
	if ((hr = ps_tgt->BltFast (tgtx, tgty, ps_src, &srcr, bltflag)) != S_OK) {
		bltflag = DDBLT_WAIT;
		if (flag & BLT_SRCCOLORKEY) bltflag |= DDBLT_KEYSRC;
		if (flag & BLT_TGTCOLORKEY) bltflag |= DDBLT_KEYDEST;
		RECT dstrct = {(LONG)tgtx, (LONG)tgty, (LONG)(tgtx+w), (LONG)(tgty+h)};
		RECT srcrct = {(LONG)srcx, (LONG)srcy, (LONG)(srcx+w), (LONG)(srcy+h)};
		hr = ps_tgt->Blt (&dstrct, ps_src, &srcrct, bltflag, NULL);
		if (hr != S_OK)
			LOGOUT_DDERR (hr);
	}
	return (hr == S_OK);
}

bool SpaceXpanseGraphics::clbkScaleBlt (SURFHANDLE tgt, DWORD tgtx, DWORD tgty, DWORD tgtw, DWORD tgth,
		                            SURFHANDLE src, DWORD srcx, DWORD srcy, DWORD srcw, DWORD srch, DWORD flag) const
{
	LPDIRECTDRAWSURFACE7 ps_tgt = (tgt ? (LPDIRECTDRAWSURFACE7)tgt : m_pddsRenderTarget);
	LPDIRECTDRAWSURFACE7 ps_src = (LPDIRECTDRAWSURFACE7)src;
	RECT srcr = {(LONG)srcx, (LONG)srcy, (LONG)(srcx+srcw), (LONG)(srcy+srch)};
	RECT tgtr = {(LONG)tgtx, (LONG)tgty, (LONG)(tgtx+tgtw), (LONG)(tgty+tgth)};
	DWORD bltflag = DDBLT_WAIT;
	if (flag & BLT_SRCCOLORKEY) bltflag |= DDBLT_KEYSRC;
	if (flag & BLT_TGTCOLORKEY) bltflag |= DDBLT_KEYDEST;
	return (ps_tgt->Blt (&tgtr, ps_src, &srcr, bltflag, 0) == S_OK);
}

bool SpaceXpanseGraphics::clbkBltCK (SURFHANDLE tgt, DWORD tgtx, DWORD tgty,
	SURFHANDLE src, DWORD srcx, DWORD srcy, DWORD w, DWORD h, DWORD ck) const
{
	return clbkScaleBltCK (tgt, tgtx, tgty, w, h, src, srcx, srcy, w, h, ck);
}

bool SpaceXpanseGraphics::clbkScaleBltCK (SURFHANDLE tgt, DWORD tgtx, DWORD tgty, DWORD tgtw, DWORD tgth,
	SURFHANDLE src, DWORD srcx, DWORD srcy, DWORD srcw, DWORD srch, DWORD ck) const
{
	RECT srcr = {(LONG)srcx, (LONG)srcy, (LONG)(srcx+srcw), (LONG)(srcy+srch)};
	RECT tgtr = {(LONG)tgtx, (LONG)tgty, (LONG)(tgtx+tgtw), (LONG)(tgty+tgth)};

	DWORD dwCC;
	static DDBLTFX ddbltfx = { 0 };
	if (!ddbltfx.dwSize) { // initialise defaults
		memset (&ddbltfx, 0, sizeof(DDBLTFX));
		ddbltfx.dwSize = sizeof(DDBLTFX);
		ddbltfx.dwROP = SRCCOPY;
	}
	switch (ck) {
	case SURF_NO_CK:     // no colour key
		dwCC = 0;
		break;
	case SURF_PREDEF_CK: // use colour key of source surface
		dwCC = DDBLT_KEYSRC;
		break;
	default:             // use ck to override colour key
		ddbltfx.ddckSrcColorkey.dwColorSpaceLowValue = ddbltfx.ddckSrcColorkey.dwColorSpaceHighValue = ck;
		dwCC = DDBLT_KEYSRCOVERRIDE;
		break;
	}

	return (((LPDIRECTDRAWSURFACE7)tgt)->Blt (&tgtr, (LPDIRECTDRAWSURFACE7)src, &srcr,
		DDBLT_WAIT | DDBLT_ROP | dwCC, &ddbltfx) == S_OK);
}

bool SpaceXpanseGraphics::clbkFillSurface (SURFHANDLE surf, DWORD col) const
{
	DDBLTFX bltfx;
	ZeroMemory (&bltfx, sizeof(bltfx));
	bltfx.dwSize = sizeof(bltfx);
	bltfx.dwFillColor = col;
	return ((LPDIRECTDRAWSURFACE7)surf)->Blt (NULL, NULL, NULL, DDBLT_COLORFILL, &bltfx) == DD_OK;
}

bool SpaceXpanseGraphics::clbkFillSurface (SURFHANDLE surf, DWORD tgtx, DWORD tgty, DWORD w, DWORD h, DWORD col) const
{
	RECT r = {(LONG)tgtx, (LONG)tgty, (LONG)(tgtx+w), (LONG)(tgty+h)};
	DDBLTFX bltfx;
	ZeroMemory (&bltfx, sizeof(bltfx));
	bltfx.dwSize = sizeof(bltfx);
	bltfx.dwFillColor = col;

	return ((LPDIRECTDRAWSURFACE7)surf)->Blt (&r, NULL, NULL, DDBLT_COLORFILL, &bltfx) == DD_OK;
}

// =======================================================================
// GDI functions
// =======================================================================

HDC SpaceXpanseGraphics::clbkGetSurfaceDC (SURFHANDLE surf)
{
	HRESULT hr;
	HDC hDC;
	LPDIRECTDRAWSURFACE7 ps = (surf ? (LPDIRECTDRAWSURFACE7)surf : m_pddsRenderTarget);
	if ((hr = ps->GetDC (&hDC)) != DD_OK)
		LOGOUT_DDERR (hr);
	return hDC;
}

void SpaceXpanseGraphics::clbkReleaseSurfaceDC (SURFHANDLE surf, HDC hDC)
{
	LPDIRECTDRAWSURFACE7 ps = (surf ? (LPDIRECTDRAWSURFACE7)surf : m_pddsRenderTarget);
	ps->ReleaseDC (hDC);
}

// =======================================================================

void SpaceXpanseGraphics::LogRenderParams () const
{
	char cbuf[256];

	sprintf (cbuf, "Viewport: %s %d x %d x %d",
		bFullscreen ? "Fullscreen":"Window", viewW, viewH, viewBPP);
	WriteLog (cbuf);
	strcpy (cbuf, "Hardware T&L capability: ");
	strcat (cbuf, GetFramework()->IsTLDevice() ? "Yes":"No");
	WriteLog (cbuf);
	if (GetFramework()->GetZBufferBitDepth()) {
		sprintf (cbuf, "Z-buffer depth: %d bit", GetFramework()->GetZBufferBitDepth());
		WriteLog (cbuf);
	}
	if (GetFramework()->GetStencilBitDepth()) {
		sprintf (cbuf, "Stencil buffer depth: %d bit", GetFramework()->GetStencilBitDepth());
		WriteLog (cbuf);
	}
	if (GetFramework()->GetMaxLights()) {
		sprintf (cbuf, "Active lights supported: %d", GetFramework()->GetMaxLights());
		WriteLog (cbuf);
	}
}

// =======================================================================

void SpaceXpanseGraphics::WriteLog (const char *msg) const
{
	static char cbuf[256] = "Graphics: ";
	strcpy (cbuf+10, msg);
	LOGOUT (cbuf);
}

