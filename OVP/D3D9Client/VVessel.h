// ==============================================================
// VVessel.h
// Part of the SPACEXPANSE VISUALISATION PROJECT (OVP)
// Dual licensed under GPL v3 and LGPL v3
// Copyright (C) 2006-2016 Martin Schweiger
//				 2012-2016 Jarmo Nikkanen
// ==============================================================

#ifndef __VVESSEL_H
#define __VVESSEL_H

#include "VObject.h"
#include "Mesh.h"
#include "gcCore.h"
#include <unordered_set>
#include <vector>

class oapi::D3D9Client;

typedef struct {
	float fdata;
	VECTOR3 ref, vdata;
	std::vector<VECTOR3> vtx;
} _defstate;



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
	friend class D3D9Client;
	/**
	 * \brief Creates a new vessel visual for a scene
	 * \param _hObj vessel object handle
	 * \param scene scene to which the visual is added
	 */
	vVessel (OBJHANDLE _hObj, const Scene *scene);

	~vVessel ();

	/**
	 * \brief Set up global parameters shared by all instances
	 * \param gclient client instance pointer
	 */
	static void GlobalInit (oapi::D3D9Client *gc);

	/**
	 * \brief Release global parameters
	 */
	static void GlobalExit ();

	void clbkEvent(DWORD evnt, DWORD_PTR context);

	MESHHANDLE GetMesh (UINT idx);
	bool GetMinMaxDistance(float *zmin, float *zmax, float *dmin);
	void GetMinMaxLightDist(float *mind, float *maxd);
	int	 GetMatrixTransform(gcCore::MatrixId matrix_id, DWORD mesh, DWORD group, FMATRIX4 *pMat);
	int  SetMatrixTransform(gcCore::MatrixId matrix_id, DWORD mesh, DWORD group, const FMATRIX4 *pMat);
	void UpdateBoundingBox();
	bool IsInsideShadows();
	bool IntersectShadowVolume();
	bool IntersectShadowTarget();
	void ReloadTextures();
	
	inline DWORD GetMeshCount();

	void PreInitObject();

	VESSEL *GetInterface() { return vessel; }

	/**
	 * \brief Per-frame object parameter updates
	 * \return \e true if update was performed, \e false if skipped.
	 * \action
	 *   - Calls vObject::Update
	 *   - Calls \ref UpdateAnimations
	 */
	bool Update (bool bMainScene);

	/**
	 * \brief Object render call
	 * \param dev render device
	 * \return \e true if render operation was performed (object active),
	 *   \e false if skipped (object inactive)
	 * \action Calls Render(dev,false), i.e. performs the external render pass.
	 * \sa Render(LPDIRECT3DDEVICE9,bool)
	 */
	bool Render (LPDIRECT3DDEVICE9 dev);

	/**
	 * \brief Object render call
	 * \param dev render device
	 * \param internalpass flag for internal render pass
	 * \note This method renders either the external vessel meshes
	 *   (internalpass=false) or internal meshes (internalpass=true), e.g.
	 *   the virtual cockpit.
	 * \note The internal pass is only performed on the focus object, and only
	 *   in cockpit camera mode.
	 * \sa Render(LPDIRECT3DDEVICE9)
	 */
	bool Render (LPDIRECT3DDEVICE9 dev, bool bInternalPass);

	bool RenderExhaust();

	/**
	 * \brief Render the vessel's active light beacons
	 * \param dev render device
	 */
	void RenderLightCone (LPD3DXMATRIX pWT);
	void RenderBeacons (LPDIRECT3DDEVICE9 dev);
	void RenderReentry (LPDIRECT3DDEVICE9 dev);
	void RenderGrapplePoints (LPDIRECT3DDEVICE9 dev);
	void RenderGroundShadow (LPDIRECT3DDEVICE9 dev, OBJHANDLE hPlanet, float depth);
	void RenderAxis (LPDIRECT3DDEVICE9 dev, D3D9Pad *pSkp);
	bool RenderENVMap (LPDIRECT3DDEVICE9 pDev, DWORD cnt=2, DWORD flags=0xFF);
	bool ProbeIrradiance(LPDIRECT3DDEVICE9 pDev, DWORD cnt = 2, DWORD flags = 0xFF);

	LPDIRECT3DCUBETEXTURE9 GetEnvMap(int idx);
	LPDIRECT3DCUBETEXTURE9 GetIrradEnv() { return pIrdEnv; }
	LPDIRECT3DTEXTURE9 GetIrradianceMap() { return pIrrad; }

	float GetExhaustLength() const { return ExhaustLength; }

	D3D9Pick Pick(const D3DXVECTOR3 *vDir);

	bool HasExtPass();
	bool HasShadow();
	bool const Playback() const { return vessel->Playback(); }
	class MatMgr * GetMaterialManager() const { return pMatMgr; }

	/**
	* \brief Update animations of the visual
	*
	* Synchronises the visual animation states with the logical
	* animation states of the vessel object.
	* \param mshidx mesh index
	* \note If mshidx == (UINT)-1 (default), all meshes are updated.
	*/
	void UpdateAnimations(int mshidx = -1);

protected:

	void LoadMeshes();
	void InsertMesh(UINT idx);
	void DisposeMeshes();
	void DelMesh(UINT idx);
	void ResetMesh(UINT idx);
	void InitNewAnimation(UINT idx);
	void DisposeAnimations();
	void DelAnimation(UINT idx);
	void ResetAnimations(UINT reset = 1);

	/**
	 * \brief Grow \ref animstate buffer (if needed)
	 *
	 * Increases the size of \ref animstate buffer (if needed).
	 * This method also updates the \ref nanim member.
	 * \param newSize new size of buffer (return value of VESSEL::GetAnimPtr())
	 * \return previous value of \ref nanim
	 */
	void GrowAnimstateBuffer (UINT newSize);

	/**
	 * \brief Modify local lighting due to planet shadow or
	 *   atmospheric dispersion.
	 * \param light pointer to D3D9Light structure receiving modified parameters
	 * \return \e true if lighting modifications should be applied, \e false
	 *   if global lighting conditions apply.
	 */
	bool ModLighting (D3D9Sun *light);

	void Animate (UINT an, UINT mshidx);
	void AnimateComponent (ANIMATIONCOMP *comp, const D3DXMATRIX &T);
	void RestoreDefaultState(ANIMATIONCOMP *AC);
	void StoreDefaultState(ANIMATIONCOMP *AC);
	void DeleteDefaultState(ANIMATIONCOMP *AC);


private:

	// Animation database containing 'default' states.
	//
	std::map<MGROUP_TRANSFORM *, _defstate> defstate;
	std::unordered_set<UINT> applyanim;
	std::map<int, double> currentstate;


	VESSEL *vessel;			// access instance for the vessel
	class MatMgr *pMatMgr;

	LPDIRECT3DCUBETEXTURE9 pEnv[4], pIrdEnv;
	LPDIRECT3DTEXTURE9 pIrrad;

	int nEnv;				// Number of environmental maps
	int iFace;				// EnvMap Face index that is to be rendered next
	int eFace;

	struct MESHREC {
		D3D9Mesh *mesh;		// DX9 mesh representation
		D3DXMATRIX *trans;	// mesh transformation matrix (rel. to vessel frame)
		WORD vismode;
	} *meshlist;			// list of associated meshes

	UINT nmesh;				// number of meshes
	UINT vClass;
	ANIMATION *anim;		// list of animations (defined in the vessel object)
	double tCheckLight;		// time for next lighting check
	float ExhaustLength;

	static class SurfNative *defreentrytex, *defexhausttex;
};

#endif // !__VVESSEL_H
