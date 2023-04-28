// Copyright (c) Martin Schweiger
// Licensed under the MIT License

// =======================================================================
// Class VObject
// Base class for all visuals

#ifndef __VOBJECT_H
#define __VOBJECT_H

#include <d3d.h>
#include "Vecmat.h"
#include "GraphicsAPI.h"

class Body;
class Scene;
class SpaceXpanseGraphics;

const unsigned long VOCAPS_HASENGINES = 1;

class VObject {
	friend class Scene;

public:
	VObject (const Body *_body);
	// Create a new visual for logical object _body

	virtual ~VObject() {}

	static void CreateDeviceObjects (SpaceXpanseGraphics *gclient);
	static void DestroyDeviceObjects ();

	virtual unsigned long GetCaps () const
	{ return 0; }

	const Body *GetBody() const { return body; }
	// the visual's logical interface

	virtual void Update (bool moving, bool force);
	// Update visual to current simulation time

	virtual void Timejump (bool moving) { Update (moving, true); }
	// Discontinuous update

	virtual void CheckResolution (double iar) = 0;
	// adjust resolution for given inverse apparent radius

	virtual bool bRenderInternal () const { return false; }

	virtual bool RenderDistRange (float &dmin, float &dmax) const { return false; }
	// objects return the near and far distance limits of the rendered object
	// (including any applied scaling)

	virtual void Render (LPDIRECT3DDEVICE7 dev) = 0;
	// Object renders itself in the 3d device

	void RenderVectors (LPDIRECT3DDEVICE7 dev);
	virtual void SetupRenderVectorList();
	// render any auxiliary vectors

	virtual void RenderExhaust (LPDIRECT3DDEVICE7, LPDIRECTDRAWSURFACE7) {}
	// all classes which define VOCAPS_HASENGINES should implement this

	virtual void RenderBeacons (LPDIRECT3DDEVICE7) {}
	// allows objects to render light beacons or similar

	bool DrawVector (LPDIRECT3DDEVICE7 dev, const Vector &end, const Vector &orig, double rad = 1.0,
		const Vector &col = Vector(1,1,1), float opac = 1.0f);

	inline double CDist () const { return cdist; }
	inline const Vector &CPos () const { return cpos; }
	inline double ScaleFactor () const { return apprad_factor; }
	inline double AppRad () const { return 1.0/iapprad; }

	inline const D3DMATRIX &MWorld() const { return mWorld; }

	virtual void clbkEvent (DWORD msg, DWORD_PTR content) {}
	// Notification of visual event (e.g. mesh addition/deletion)

protected:
	static SpaceXpanseGraphics *gc;
	// inline graphics client instance

	void RenderAsPixel (LPDIRECT3DDEVICE7 dev);
	// Render distant object as 2x2 pixel block

	void RenderAsDisc (LPDIRECT3DDEVICE7 dev);
	// Render distant object as a billboard disc texture

	void RenderSpot (LPDIRECT3DDEVICE7 dev, const Vector *ofs, float size, const Vector &col, bool lighting, int shape = 0);
	// Render a spot with the given parameters (representing either a
	// complete object, or a beacon etc.)

	void RenderAsSpot (LPDIRECT3DDEVICE7 dev, D3DCOLORVALUE *illumination = 0);
	// Render distant object as circular blob with a billboard texture

	const Body *body;    // reference to logical object
	static Scene *scene; // reference to scene
	D3DMATRIX mWorld;    // world transform matrix
	MATRIX4 dmWorld;     // world transformation matrix in double precision

	Vector cpos;         // object position relative to camera
	double cdist;        // current distance from camera
	double iapprad;      // inverse of apparent radius

private:
	double apprad_factor; // auxiliary variable for apprad calculation
	double isdist_old;    // old inverse distance camera -> object surface
	static LPDIRECTDRAWSURFACE7 blobtex[3]; // billboard blob texture for distant objects
};

// =======================================================================
// nonmember definitions

struct vecentry {  // structure for rendering vectors
	Vector v;
	Vector orig;
	double rad;
	double dist;
	Vector col;
	float alpha;
	char label[32];
	DWORD lcol;
	float lsize;
	vecentry *next;
};

void AddVec (const Vector &cam, const Vector &v, const Vector &orig, double rad, const Vector &col, float alpha = 1.0, char *label = 0, DWORD lcol = 0, float lsize = -1.0);
// add a vector to the render list
// cam: camera position in local object frame
// v: vector, rad: line radius, col: colour, alpha: opacity, label: label, lcol: label colour (0=same as vector), lsize: label size (-1=default)

#endif // !__VOBJECT_H