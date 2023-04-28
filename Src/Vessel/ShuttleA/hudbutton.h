// Copyright (c) Martin Schweiger
// Licensed under the MIT License

// ==============================================================
//                 SPACEXPANSE MODULE: ShuttleA
//                  Part of the SPACEXPANSE SDK
//
// hudbutton.h
// User interface for HUD button controls
// ==============================================================

#ifndef __HUDBUTTON_H
#define __HUDBUTTON_H

#include "..\Common\Instrument.h"

// ==============================================================

class HUDButton: public PanelElement {
public:
	HUDButton (VESSEL3 *v);
	void AddMeshData2D (MESHHANDLE hMesh, DWORD grpidx);
	bool Redraw2D (SURFHANDLE surf);
	bool ProcessMouse2D (int event, int mx, int my);
};

#endif // !__HUDBUTTON_H