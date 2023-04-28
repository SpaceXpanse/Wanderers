// Copyright (c) Martin Schweiger
// Licensed under the MIT License

// ==============================================================
//                 SPACEXPANSE MODULE: ShuttleA
//                  Part of the SPACEXPANSE SDK
//
// InstrVs.h
// Vertical speed tape instrument for the ShuttleA
// ==============================================================

#ifndef __INSTRVS_H
#define __INSTRVS_H

#include "..\Common\Instrument.h"

// ==============================================================

class InstrSpd: public PanelElement {
public:
	InstrSpd (VESSEL3 *v);
	void AddMeshData2D (MESHHANDLE hMesh, DWORD grpidx);
	bool Redraw2D (SURFHANDLE surf);

private:
	int psmin;
};

// ==============================================================

class InstrAlt: public PanelElement {
public:
	InstrAlt (VESSEL3 *v);
	void AddMeshData2D (MESHHANDLE hMesh, DWORD grpidx);
	bool Redraw2D (SURFHANDLE surf);

private:
	int pamin;
};

// ==============================================================

class InstrVS: public PanelElement {
public:
	InstrVS (VESSEL3 *v);
	void AddMeshData2D (MESHHANDLE hMesh, DWORD grpidx);
	bool Redraw2D (SURFHANDLE surf);

private:
	int pvmin;
};

// ==============================================================

class InstrVAcc: public PanelElement {
public:
	InstrVAcc (VESSEL3 *v);
	void AddMeshData2D (MESHHANDLE hMesh, DWORD grpidx);
	bool Redraw2D (SURFHANDLE surf);

private:
	double pvspd;
	double pt;
};

#endif // !__INSTRVS_H