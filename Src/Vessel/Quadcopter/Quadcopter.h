// Copyright (c) Martin Schweiger
// Licensed under the MIT License

// ==============================================================
//                SPACEXPANSE MODULE: Quadcopter
//                  Part of the SPACEXPANSE SDK
//
// Quadcopter.h
// Class interface for quadcopter vessel class module
// ==============================================================

#ifndef __QUADCOPTER_H
#define __QUADCOPTER_H

#include "spacexpansesdk.h"
#include "QuadcopterLua.h"
#include "..\Common\Instrument.h"

class PropulsionSubsystem;

class Quadcopter : public ComponentVessel
{
public:
	Quadcopter(OBJHANDLE hObj, int fmodel);

	PropulsionSubsystem *ssysPropulsion() { return ssys_propulsion; }

	void clbkSetClassCaps(FILEHANDLE cfg);
	int clbkGeneric(int msgid, int prm, void *context);

private:
	// Vessel subsystems
	PropulsionSubsystem *ssys_propulsion;
	LuaInterface lua;

	MESHHANDLE exmesh_tpl;
	PROPELLANT_HANDLE ph;
	THRUSTER_HANDLE th;
	THGROUP_HANDLE tgh;
};

#endif // !__QUADCOPTER_H
