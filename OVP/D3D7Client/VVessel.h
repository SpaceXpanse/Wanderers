// Copyright (c) Martin Schweiger
// Licensed under the MIT License

// ==============================================================
//   SPACEXPANSE VISUALISATION PROJECT (OVP)
//   D3D7 Client module
// ==============================================================

// ==============================================================
// VVessel.h
// Vessel visualisation
// ==============================================================

#ifndef __VVESSEL_H
#define __VVESSEL_H

#include "VObject.h"
#include "Mesh.h"

class oapi::D3D7Client;

// ==============================================================
// class vVessel (interface)
// ==============================================================
/**
 * \brief Visual representation of a vessel object.
 *
 * The vVessel instance is not persistent: It is created when the
 * object moves into visual range of the camera, and is destroyed
 * when it moves out of visual range.
 *
 * The vVessel contains a set of meshes and animations. At each
 * time step, it synchronises the visual with the logical animation
 * state, and renders the resulting meshes.
 */

class vVessel: public vObject {
public:
	/**
	 * \brief Creates a new vessel visual for a scene
	 * \param _hObj vessel object handle
	 * \param scene scene to which the visual is added
	 */
	vVessel (OBJHANDLE _hObj, const Scene *scene);

	~vVessel ();

	static void GlobalInit (oapi::D3D7Client *gc);
	static void GlobalExit ();

	void clbkEvent (DWORD event, DWORD_PTR context);

	virtual MESHHANDLE GetMesh (UINT idx);

	/**
	 * \brief Per-frame object parameter updates
	 * \return \e true if update was performed, \e false if skipped.
	 * \action
	 *   - Calls vObject::Update
	 *   - Calls \ref UpdateAnimations
	 */
	bool Update ();

	/**
	 * \brief Object render call
	 * \param dev render device
	 * \return \e true if render operation was performed (object active),
	 *   \e false if skipped (object inactive)
	 * \action Calls Render(dev,false), i.e. performs the external render pass.
	 * \sa Render(LPDIRECT3DDEVICE7,bool)
	 */
	bool Render (LPDIRECT3DDEVICE7 dev);

	/**
	 * \brief Object render call
	 * \param dev render device
	 * \param internalpass flag for internal render pass
	 * \note This method renders either the external vessel meshes
	 *   (internalpass=false) or internal meshes (internalpass=true), e.g.
	 *   the virtual cockpit.
	 * \note The internal pass is only performed on the focus object, and only
	 *   in cockpit camera mode.
	 * \sa Render(LPDIRECT3DDEVICE7)
	 */
	bool Render (LPDIRECT3DDEVICE7 dev, bool internalpass);

	bool RenderExhaust (LPDIRECT3DDEVICE7 dev);

	/**
	 * \brief Render the vessel's active light beacons
	 * \param dev render device
	 */
	void RenderBeacons (LPDIRECT3DDEVICE7 dev);

	void RenderGroundShadow (LPDIRECT3DDEVICE7 dev, OBJHANDLE hPlanet);

protected:
	void LoadMeshes();
	void InsertMesh (UINT idx);
	void ClearMeshes();
	void DelMesh (UINT idx);
	void InitAnimations();
	void ClearAnimations();

	void SetExhaustVertices (const VECTOR3 &edir, const VECTOR3 &cdir, const VECTOR3 &ref,
		double lscale, double wscale, VERTEX_XYZ_TEX *ev);

	/**
	 * \brief Update animations of the visual
	 *
	 * Synchronises the visual animation states with the logical
	 * animation states of the vessel object.
	 * \param mshidx mesh index
	 * \note If mshidx == (UINT)-1 (default), all meshes are updated.
	 */
	void UpdateAnimations (UINT mshidx = (UINT)-1);

	/**
	 * \brief Modify local lighting due to planet shadow or
	 *   atmospheric dispersion.
	 * \param light pointer to D3DLIGHT7 structure receiving modified parameters
	 * \return \e true if lighting modifications should be applied, \e false
	 *   if global lighting conditions apply.
	 */
	bool ModLighting (LPD3DLIGHT7 light);

	void Animate (UINT an, double state, UINT mshidx);
	void AnimateComponent (ANIMATIONCOMP *comp, const D3DMATRIX &T);

private:
	VESSEL *vessel;       // access instance for the vessel
	struct MESHREC {
		D3D7Mesh *mesh;   // DX7 mesh representation
		D3DMATRIX *trans; // mesh transformation matrix (rel. to vessel frame)
		WORD vismode;
	} *meshlist;          // list of associated meshes
	UINT nmesh;           // number of meshes

	ANIMATION *anim;      // list of animations (defined in the vessel object)
	double *animstate;    // list of visual animation states
	UINT nanim;           // number of animations
	static LPDIRECTDRAWSURFACE7 mfdsurf; // texture for rendering MFDs and HUDs
	static LPDIRECTDRAWSURFACE7 defexhausttex; // default exhaust texture
	double tCheckLight;    // time for next lighting check
	bool bLocalLight;      // modified local lighting parameters?
	D3DLIGHT7 localLight;  // current local lighting parameters
};

#endif // !__VVESSEL_H