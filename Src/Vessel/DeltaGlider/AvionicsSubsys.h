// Copyright (c) Martin Schweiger
// Licensed under the MIT License

// ==============================================================
//                SPACEXPANSE MODULE: DeltaGlider
//                  Part of the SPACEXPANSE SDK
//
// AvionicsSubsys.h
// Subsystem for avionics components:
// - angular rate indicator
// ==============================================================

#ifndef __AVIONICSSUBSYS_H
#define __AVIONICSSUBSYS_H

#include "DGSubsys.h"

// ==============================================================
// Avionics subsystem
// ==============================================================

class InstrAtt;
class InstrHSI;
class InstrAOA;
class InstrVS;
class FuelMFD;
class AngRateIndicator;
class AAPSubsystem;

class AvionicsSubsystem: public DGSubsystem {
public:
	AvionicsSubsystem (DeltaGlider *v);
	bool clbkLoadPanel2D (int panelid, PANELHANDLE hPanel, DWORD viewW, DWORD viewH);
	bool clbkLoadVC (int vcid);

private:
	InstrAtt *instratt;
	InstrHSI *instrhsi;
	InstrAOA *instraoa;
	InstrVS  *instrvs;
	FuelMFD  *fuelmfd;
	AngRateIndicator *angrateind;
	AAPSubsystem *aapssys;
	int ELID_INSTRATT;
	int ELID_INSTRHSI;
	int ELID_INSTRAOA;
	int ELID_INSTRVS;
	int ELID_FUELMFD;
	int ELID_ANGRATEIND;
};

#endif // !__AVIONICSSUBSYS_H