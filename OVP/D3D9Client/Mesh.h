// ==============================================================
// Mesh.h
// Part of the SPACEXPANSE VISUALISATION PROJECT (OVP)
// Dual licensed under GPL v3 and LGPL v3
// Copyright (C) 2006-2016 Martin Schweiger
//				 2012-2019 Jarmo Nikkanen
// ==============================================================

// ==============================================================
// class D3D9Mesh (interface)
//
// This class represents a mesh in terms of DX7 interface elements
// (vertex buffers, index lists, materials, textures) which allow
// it to be rendered to the D3D9 device.
// ==============================================================

#ifndef __MESH_H
#define __MESH_H

#include "D3D9Client.h"
#include "D3D9Effect.h"
#include "AABBUtil.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>

const DWORD SPEC_DEFAULT = (DWORD)(-1); // "default" material/texture flag
const DWORD SPEC_INHERIT = (DWORD)(-2); // "inherit" material/texture flag

#define RENDER_VESSEL		0
#define RENDER_BASE			1
#define RENDER_ASTEROID		2
#define RENDER_BASETILES	3
#define RENDER_VC			4
#define RENDER_BASEBS		5
#define RENDER_CUSTOM		6

#define ENVMAP_MAIN			0



#define SHADER_NULL			0xFFFF
#define SHADER_PBR			0
#define SHADER_ADV			1
#define SHADER_LEGACY		2			// Shader most compatible with DX7 Inline
#define SHADER_XR2HUD		3			// XR2 HUD shader
#define SHADER_METALNESS	4

#define VCLASS_AMSO			1
#define VCLASS_XR2			2
#define VCLASS_ULTRA		3
#define VCLASS_SSU_CENTAUR	4

#define MAPMODE_UNKNOWN		0
#define MAPMODE_CURRENT		1
#define MAPMODE_STATIC		2
#define MAPMODE_DYNAMIC		3


struct _LightList {
	int		idx;
	float	illuminace;
};





class MeshBuffer
{
public:

	MeshBuffer(MeshBuffer *pSrc, const class D3D9Mesh *_pRoot);
	MeshBuffer(DWORD nVtx, DWORD nIdx, const class D3D9Mesh *_pRoot);
	~MeshBuffer();

	void Map(LPDIRECT3DDEVICE9 pDev);
	bool IsLocalTo(const class D3D9Mesh *_pRoot) const { return (_pRoot == pRoot); }
	void MustRemap(DWORD mode);

	LPDIRECT3DVERTEXBUFFER9 pVB;
	LPDIRECT3DVERTEXBUFFER9 pGB;
	LPDIRECT3DINDEXBUFFER9  pIB;

	NMVERTEX				*pVBSys;
	D3DXVECTOR4				*pGBSys;
	WORD					*pIBSys;

	DWORD nVtx;
	DWORD nIdx;
	DWORD mapMode;
	bool  bMustRemap;

	const class D3D9Mesh	*pRoot;
};






/**
 * \brief Mesh object with D3D9-specific vertex buffer
 *
 * Meshes consist of one or more vertex groups, and a set of materials and
 * textures.
 */

class D3D9Mesh : private D3D9Effect
{

public:

	bool bCanRenderFast;		// Mesh doesn't contain any advanced features in any group
	bool bIsReflective;			// Mesh has a reflective material in one or more groups
	bool bMtrlModidied;
	bool bIsTemplate;
	
	D9BBox BBox;
	MeshBuffer *pBuf;

	struct GROUPREC {			// mesh group definition
		DWORD VertOff;			// Main mesh Vertex Offset
		DWORD IdexOff;			// Main mesh Index Offset
		//------------------------------------------------
		DWORD nFace;			// Face/Primitive count
		DWORD nVert;			// Vertex count
		//------------------------------------------------
		DWORD MtrlIdx;			// material index
		DWORD TexIdx;			// texture index 0=None
		DWORD UsrFlag;			// user-defined flag
		WORD  IntFlag;			// internal flags
		WORD  zBias;
		WORD  MFDScreenId;		// MFD screen ID + 1
		WORD  PBRStatus;
		WORD  Shader;
		bool  bTransform;
		bool  bUpdate;			// Bounding box update required
		bool  bDualSided;
		bool  bDeleted;			// This entry is deleted by DelGroup()
		bool  bRendered;
		D3DXMATRIX  Transform;	// Group specific transformation matrix
		D9BBox BBox;
		DWORD TexIdxEx[MAXTEX];
		float TexMixEx[MAXTEX];
	};


					D3D9Mesh(const char *fname);
					D3D9Mesh(DWORD nGrp, const MESHGROUPEX **hGroup, const SURFHANDLE *hSurf);
					D3D9Mesh(const MESHGROUPEX *pGroup, const MATERIAL *pMat, SurfNative *pTex);
					D3D9Mesh(MESHHANDLE hMesh, bool asTemplate = false, D3DXVECTOR3 *reorig = NULL, float *scale = NULL);
					D3D9Mesh(MESHHANDLE hMesh, const D3D9Mesh &hTemp);
					~D3D9Mesh();

	bool			IsOK() const { return pBuf != NULL; }

	void			Release();

	void			LoadMeshFromHandle(MESHHANDLE hMesh, D3DXVECTOR3 *reorig = NULL, float *scale = NULL);
	void			ReLoadMeshFromHandle(MESHHANDLE hMesh);
	void			ReloadTextures();

	void			SetName(const char *name);
	void			SetName(UINT idx);
	const char *	GetName() const { return name; }

	void			SetDefaultShader(WORD shader) { DefShader = shader; bMtrlModidied = true; }
	WORD			GetDefaultShader() const { return DefShader; }
	
	void			SetClass(DWORD cl) { vClass = cl; }


	/**
	 * \brief Check if a mesh is casting shadows
	 * \return Returns true if the mesh is casting shadows.
	 */
	bool			HasShadow() const;

	/**
	 * \brief Returns a pointer to a mesh group.
	 * \param idx group index (>= 0)
	 * \return Pointer to group structure.
	 */
	const GROUPREC * GetGroup(DWORD idx) const;
	void            SetMFDScreenId(DWORD idx, WORD id);
	void			SetDualSided(DWORD idx, bool bState) { Grp[idx].bDualSided = bState; }

	/**
	 * \brief Returns number of material specifications.
	 * \return Number of materials.
	 */
	SURFHANDLE		GetTexture(DWORD idx) const { return (SURFHANDLE)Tex[idx]; }
	bool			HasTexture(SURFHANDLE hSurf) const;
	bool			IsReflective() const { return bIsReflective | (DefShader==SHADER_METALNESS); }

	/**
	 * \brief returns a pointer to a material definition.
	 * \param idx material index (>= 0)
	 * \return Pointer to material object.
	 */
	const D3D9MatExt *	GetMaterial(DWORD idx) const;
	bool			GetMaterial(D3D9MatExt *pMat, DWORD idx) const;
	void			SetMaterial(const D3D9MatExt *pMat, DWORD idx, bool bUpdateStatus = true);
	void			SetMaterial(const D3DMATERIAL9 *pMat, DWORD idx, bool bUpdateStatus = true);
	int				SetMaterialEx(DWORD idx, MatProp mid, const FVECTOR4* in);
	int				GetMaterialEx(DWORD idx, MatProp mid, FVECTOR4* out);
	bool			GetTexTune(D3D9Tune *pT, DWORD idx) const;
	void			SetTexTune(const D3D9Tune *pT, DWORD idx);

	DWORD			GetGroupCount() const { return nGrp; }
	DWORD			GetMaterialCount() const { return nMtrl; }
	DWORD			GetTextureCount() const { return nTex; }
	DWORD			GetVertexCount(int grp=-1) const;
	DWORD			GetIndexCount(int grp=-1) const;
	bool			IsGroupRendered(DWORD grp) const;

	DWORD			GetMeshGroupMaterialIdx(DWORD grp) const;
	DWORD			GetMeshGroupTextureIdx(DWORD grp) const;
	DWORD			GetGroupTransformCount() const;
	D3DXVECTOR3		GetBoundingSpherePos();
	float			GetBoundingSphereRadius();
	D9BBox *		GetAABB();
	D3DXVECTOR3		GetGroupSize(DWORD idx) const;
	LPD3DXMATRIX	GetTransform() { if (bGlobalTF) return &mTransform; else return NULL; }

	D3DXMATRIX		GetTransform(int grp, bool bCombined);
	bool			SetTransform(int grp, const LPD3DXMATRIX pMat);

	void			SetPosition(VECTOR3 &pos);
	void			SetRotation(D3DXMATRIX &rot);

	/**
	 * \brief Replace a mesh texture.
	 * \param texidx texture index (>= 0)
	 * \param tex texture handle
	 * \return \e true on success, \e false otherwise.
	 */
	bool			SetTexture(DWORD texidx, SURFHANDLE tex);
	void			SetTexMixture (DWORD ntex, float mix);

	void			RenderGroup(const GROUPREC *grp);
	void			RenderGroup(int idx);
	void			RenderBaseTile(const LPD3DXMATRIX pW);
	void			RenderBoundingBox(const LPD3DXMATRIX pW);
	void			Render(const LPD3DXMATRIX pW, int iTech=RENDER_VESSEL, LPDIRECT3DCUBETEXTURE9 *pEnv=NULL, int nEnv=0);
	void			RenderFast(const LPD3DXMATRIX pW, int iTech);
	void			RenderShadows(float alpha, const LPD3DXMATRIX pP, const LPD3DXMATRIX pW, bool bShadowMap = false, const D3DXVECTOR4 *elev = NULL);
	void			RenderShadowsEx(float alpha, const LPD3DXMATRIX pP, const LPD3DXMATRIX pW, const D3DXVECTOR4 *light, const D3DXVECTOR4 *param);
	void			RenderRings(const LPD3DXMATRIX pW, LPDIRECT3DTEXTURE9 pTex);
	void			RenderRings2(const LPD3DXMATRIX pW, LPDIRECT3DTEXTURE9 pTex, float irad, float orad);
	void			RenderAxisVector(LPD3DXMATRIX pW, const LPD3DXCOLOR pColor, float len);
	void			RenderSimplified(const LPD3DXMATRIX pW, LPDIRECT3DCUBETEXTURE9 *pEnv = NULL, int nEnv = 0, bool bSP = false);
	void			CheckMeshStatus();
	void			ResetTransformations();
	void			TransformGroup(DWORD n, const D3DXMATRIX *m);
	void			Transform(const D3DXMATRIX *m);
	int				GetGroup (DWORD grp, GROUPREQUESTSPEC *grs);
	int				EditGroup (DWORD grp, GROUPEDITSPEC *ges);

	void			SetSunLight(const D3D9Sun *pLight);

	D3D9Pick		Pick(const LPD3DXMATRIX pW, const LPD3DXMATRIX pT, const D3DXVECTOR3 *vDir);

	void			UpdateBoundingBox();
	void			BoundingBox(const NMVERTEX *vtx, DWORD n, D9BBox *box);

	void			SetAmbientColor(D3DCOLOR c);
	void			SetupFog(const LPD3DXMATRIX pW);
	void			ResetRenderStatus();


	/**
	 * \brief Enable/disable material alpha value for transparency calculation.
	 * \param enable flag for enabling/disabling material alpha calculation.
	 * \note By default, material alpha values are ignored for mesh groups
	 *   with textures, and the texture alpha values are used instead.
	 *   By enabling material alpha calculation, the final alpha value is
	 *   calculated as the product of material and texture alpha value.
	 */
	inline void		EnableMatAlpha (bool enable) { bModulateMatAlpha = enable; }

private:


	void			UpdateTangentSpace(NMVERTEX *pVrt, WORD *pIdx, DWORD nVtx, DWORD nFace, bool bTextured);
	void			ProcessInherit();
	bool			CopyVertices(GROUPREC *grp, const MESHGROUPEX *mg, D3DXVECTOR3 *reorig = NULL, float *scale = NULL);
	void			SetGroupRec(DWORD i, const MESHGROUPEX *mg);
	void			Null(const char *meshName = NULL);


	WORD	DefShader;
	DWORD	MaxVert;
	DWORD	MaxFace;
	GROUPREC *Grp;              // list of mesh groups
	DWORD nGrp;                 // number of mesh groups
	DWORD nMtrl;                // number of mesh materials
	DWORD nTex;                 // number of mesh textures
	DWORD vClass;
	D3D9MatExt *Mtrl;           // list of mesh materials
	SurfNative **Tex;			// list of mesh textures
	D3D9Tune *pTune;
	D3DXMATRIX mTransform;
	D3DXMATRIX mTransformInv;
	D3DXMATRIX *pGrpTF;
	const D3D9Sun *sunLight;
	D3DCOLOR cAmbient;
	LightStruct null_light;

	_LightList LightList[MAX_SCENE_LIGHTS];
	LightStruct *Locals;
	bool bBSRecompute;			// Bounding sphere must be recomputed
	bool bBSRecomputeAll;
	bool bModulateMatAlpha;     // mix material and texture alpha channels
	bool bGlobalTF;				// Mesh has a valid mTransform matrix

	char name[128];


};

#endif // !__MESH_H
