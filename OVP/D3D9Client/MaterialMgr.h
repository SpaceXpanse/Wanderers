// ==============================================================
// MaterialMgr.h
// Part of the SPACEXPANSE VISUALISATION PROJECT (OVP)
// Dual licensed under GPL v3 and LGPL v3
// Copyright (C) 2012 - 2016 Jarmo Nikkanen
// ==============================================================

#ifndef __MATERIALMGR_H
#define __MATERIALMGR_H

#include <d3d9.h>
#include <d3dx9.h>

#include "Mesh.h"
#include "D3D9Client.h"
#include "D3D9Util.h"
#include "vObject.h"

#define ENVCAM_OMIT_ATTC		0x0001
#define ENVCAM_OMIT_DOCKS		0x0002
#define ENVCAM_FOCUS			0x0004


/**
 * \brief Storage structure to keep environmental camera information.
 */
struct ENVCAMREC {
	D3DXVECTOR3		lPos;			///< Camera local position
	float			near_clip;		///< Near clip-plane distance
	DWORD			flags;			///< Camera flags
	WORD			nAttc;			///< Number of attachments points in a list
	WORD			nDock;			///< Number of docking ports in a list
	BYTE *			pOmitAttc;		///< Omit attachments
	BYTE *			pOmitDock;		///< Omit cessels in docking ports
};

/**
 * \brief Management of custom configurations for vessel materials
 */
class MatMgr {

public:
	// Disable copy construct & copy assign
					MatMgr    (MatMgr const&) = delete;
	MatMgr &		operator= (MatMgr const&) = delete;

					MatMgr(class vObject *vObj, class D3D9Client *_gc);
					~MatMgr();

	//DWORD			NewRecord(const char *name, DWORD midx);
	//void			ClearRecord(DWORD iRec);
	void			RegisterMaterialChange(D3D9Mesh *pMesh, DWORD midx, const D3D9MatExt *pM);
	void			RegisterShaderChange(D3D9Mesh *pMesh, WORD id);
	void			ApplyConfiguration(D3D9Mesh *pMesh);
	bool			SaveConfiguration();
	bool			LoadConfiguration(bool bAppend=false);
	bool			LoadCameraConfig();
	bool			HasMesh(const char *name);
	void			ResetCamera(DWORD idx);

	ENVCAMREC *		GetCamera(DWORD idx);
	DWORD			CameraCount();

private:

	vObject			*vObj;
	D3D9Client		*gc;
	//DWORD			nRec;	///< Number of records
	//DWORD			mRec;	///< Allocated records


	struct SHADER {
		SHADER(string x, WORD i) { name = x; id = i; }
		string name;
		WORD id;
	};

	struct MESHREC {
		WORD shader;
		map<int, D3D9MatExt> material;
	};

	std::map<string, MESHREC> MeshConfig;
	std::list<SHADER> Shaders;

	ENVCAMREC *pCamera;
};

#endif
