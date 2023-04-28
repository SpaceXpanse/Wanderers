// Copyright (c) Martin Schweiger
// Licensed under the MIT License

// ==============================================================
//   SPACEXPANSE VISUALISATION PROJECT (OVP)
//   D3D7 Client module
// ==============================================================

// ==============================================================
// Scene.h
// Class Scene (interface)
//
// A "Scene" represents the 3-D world as seen from a specific
// viewpoint ("camera"). Each scene therefore has a camera object
// associated with it. The SpaceXpanse core supports a single
// camera, but in principle a graphics client could define
// multiple scenes and render them simultaneously into separate
// windows (or into MFD display surfaces, etc.)
// ==============================================================

#ifndef __SCENE_H
#define __SCENE_H

#include "D3D7Client.h"
#include "CelSphere.h"
#include "VObject.h"
#include "Light.h"

class CSphereManager;
class vObject;
class D3D7ParticleStream;

class Scene {
	friend class Camera;

public:
	Scene (oapi::D3D7Client *_gc, DWORD w, DWORD h);
	~Scene ();

	inline const oapi::D3D7Client *GetClient() const { return gc; }
	// return the client

	inline Camera *GetCamera() const { return cam; }
	// return associated camera

	const D3DLIGHT7 *GetLight() const;

	inline DWORD GetStencilDepth() const { return stencilDepth; }

	inline D3DCOLOR GetBgColour() const { return bg_rgba; }
	// ambient background colour

	inline const DWORD ViewW() const { return viewW; }
	inline const DWORD ViewH() const { return viewH; }
	// return viewport dimensions

	void CheckVisual (OBJHANDLE hObj);
	// checks if hObj is within visual range, and creates or
	// deletes the associated vObject as required.

	void Initialise ();

	void Update ();
	// Update camera position, visuals, etc.

	void Render ();
	// Render the scene

	/**
	 * \brief Render any shadows cast by vessels on planet surfaces
	 * \param hPlanet handle of planet to cast shadows on
	 * \param depth shadow darkness parameter (0=none, 1=black)
	 * \note Uses stencil buffering if available and requested. Otherwise shadows
	 *   are pure black.
	 * \note Requests for any planet other than that closest to the camera
	 *   are ignored.
	 */
	void RenderVesselShadows (OBJHANDLE hPlanet, float depth) const;

	/**
	 * \brief Create a visual for a new vessel if within visual range.
	 * \param hVessel vessel object handle
	 */
	void NewVessel (OBJHANDLE hVessel);

	/**
	 * \brief Delete a vessel visual prior to destruction of the logical vessel.
	 * \param hVessel vessel object handle
	 */
	void DeleteVessel (OBJHANDLE hVessel);

	void AddParticleStream (D3D7ParticleStream *_pstream);
	void DelParticleStream (DWORD idx);

protected:
	/**
	 * \brief Return a render window device context for drawing markers.
	 * \param mode marker mode
	 * \return Drawing device context
	 */
	HDC GetLabelDC (int mode);

	/**
	 * \brief Render a single marker for a given direction
	 * \param hDC device context
	 * \param rdir normalised direction from camera in global (ecliptic) frame
	 * \param label1 label above marker
	 * \param label2 label below marker
	 * \param mode marker shape
	 * \param scale marker size
	 */
	void RenderDirectionMarker (HDC hDC, const VECTOR3 &rdir, const char *label1, const char *label2, int mode, int scale);

	/**
	 * \brief Render a single marker at a given global position
	 * \param hDC device context
	 * \param gpos global position (ecliptic frame)
	 * \param label1 label above marker
	 * \param label2 label below marker
	 * \param mode marker shape
	 * \param scale marker size
	 */
	void RenderObjectMarker (HDC hDC, const VECTOR3 &gpos, const char *label1, const char *label2, int mode, int scale);

	void AddLocalLight (const LightEmitter *le, const vObject *vo, DWORD idx);

private:
	oapi::D3D7Client *gc;
	LPDIRECT3DDEVICE7 dev;     // render device
	DWORD viewW, viewH;        // render viewport size
	DWORD stencilDepth;        // stencil buffer bit depth
	Camera *cam;               // camera object
	CelestialSphere *csphere;  // celestial sphere background
	DWORD iVCheck;             // index of last object checked for visibility
	DWORD zclearflag;          // z and stencil buffer clear flag

	D3D7ParticleStream **pstream; // list of particle streams
	DWORD                nstream; // number of streams

	D3D7Light *light;          // only one for now
	D3DCOLOR bg_rgba;          // ambient background colour
	bool locallight;           // enable local light sources
	bool surfLabelsActive;     // v.2 surface labels activated?
	DWORD maxlight;            // max number of light sources

	struct VOBJREC {           // linked list of object visuals
		vObject *vobj;         // visual instance
		VOBJREC *prev, *next;  // previous and next list entry
	} *vobjFirst, *vobjLast;   // first and last list entry

	struct LIGHTLIST {
		const LightEmitter *plight;
		vObject *vobj;
		double camdist2;
	} *lightlist;

	VOBJREC *FindVisual (OBJHANDLE hObj);
	// Locate the visual for hObj in the list if present, or return
	// NULL if not found

	void DelVisualRec (VOBJREC *pv);
	// Delete entry pv from the list of visuals

	VOBJREC *AddVisualRec (OBJHANDLE hObj);
	// Add an entry for object hObj in the list of visuals

	VECTOR3 SkyColour ();
	// Sky background colour based on atmospheric parameters of closest planet

	// GDI resources
	static COLORREF labelCol[6];
	HPEN hLabelPen[6];
	HFONT hLabelFont[1];
	int labelSize[1];
	oapi::Font *label_font[4];
	oapi::Pen *label_pen;

	void InitGDIResources();
	void ExitGDIResources();

	CSphereManager *cspheremgr;
};

#endif // !__SCENE_H