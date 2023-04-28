// ==============================================================
// MeshMgr.h
// Part of the SPACEXPANSE VISUALISATION PROJECT (OVP)
// Dual licensed under GPL v3 and LGPL v3
// Copyright (C) 2006-2016 Martin Schweiger
// ==============================================================

#ifndef __MESHMGR_H
#define __MESHMGR_H

#include "D3D9Client.h"
#include "Mesh.h"

// ==============================================================
// class MeshManager (interface)
// ==============================================================
/**
 * \brief Simple management of persistent mesh templates
 */

class MeshManager {
public:
	explicit MeshManager (oapi::D3D9Client *gclient);
	~MeshManager();
	void DeleteAll();
	int StoreMesh (MESHHANDLE hMesh, const char *name);
	const D3D9Mesh *GetMesh (MESHHANDLE hMesh);

private:
	oapi::D3D9Client *gc;
	struct MeshBuffer {
		MESHHANDLE hMesh;
		D3D9Mesh *mesh;
	} *mlist;
	int nmlist, nmlistbuf;
};

#endif // !__MESHMGR_H