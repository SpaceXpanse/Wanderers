// Copyright (c) Martin Schweiger
// Licensed under the MIT License

#ifndef __UTIL_H
#define __UTIL_H

#include <windows.h>
#include "Vecmat.h"
#include "SpaceXpanseAPI.h"
#include "SpaceXpanse.h"

extern SpaceXpanse *g_pSpaceXpanse;

LONGLONG NameToId (const char *name);
// converts a file name into an identifier. Note that this is not guaranteed to be unique.

// should go into the graphics client
inline DWORD GetSurfColour (DWORD r, DWORD g, DWORD b)
{
	DWORD bpp = g_pSpaceXpanse->ViewBPP();
	if (bpp >= 24) return (r << 16) + (g << 8) + b;
	else           return (((r*319)/2559) << 11) + (((g*639)/2559) << 5) + ((b*319)/2559);
}

DWORDLONG Str2Crc (const char *str);
// converts str into an integer identifier (not guaranteed unique!)

// simple string de-scrambling routine
char *uscram (const char *str);

// create the directories 
bool MakePath (const char *fname);

// case-insensitive comparison of std::strings
bool iequal(const std::string& s1, const std::string& s2);

// conversion between Vector and VECTOR3 structures

inline Vector MakeVector (const VECTOR3 &v)
{ return Vector(v.x, v.y, v.z); }

inline VECTOR3 MakeVECTOR3 (const Vector &v)
{ return _V(v.x, v.y, v.z); }

inline VECTOR4 MakeVECTOR4 (const Vector &v)
{ return _V(v.x, v.y, v.z, 1.0); }

inline void CopyVector (const VECTOR3 &v3, Vector &v)
{ v.x = v3.x, v.y = v3.y, v.z = v3.z; }

inline void CopyVector (const Vector &v, VECTOR3 &v3)
{ v3.x = v.x, v3.y = v.y, v3.z = v.z; }

inline void EulerAngles (const Matrix &R, Vector &e)
{
	e.x = atan2 (R.m23, R.m33);
	e.y = -asin (R.m13);
	e.z = atan2 (R.m12, R.m11);
}

inline void EulerAngles (const Matrix &R, VECTOR3 &e)
{
	e.x = atan2 (R.m23, R.m33);
	e.y = -asin (R.m13);
	e.z = atan2 (R.m12, R.m11);
}

double rand1();
// uniformly distributed random number, range [0,1]

// Timing functions
void tic();   // start clock
double toc(); // stop clock and return value

RECT GetClientPos (HWND hWnd, HWND hChild);
void SetClientPos (HWND hWnd, HWND hChild, RECT &r);

#endif //!__UTIL_H