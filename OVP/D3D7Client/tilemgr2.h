// Copyright (c) Martin Schweiger
// Licensed under the MIT License

// ==============================================================
//   SPACEXPANSE VISUALISATION PROJECT (OVP)
//   D3D7 Client module
// ==============================================================

// =======================================================================
// tilemgr2.h
// Rendering of planetary surfaces using texture tiles at
// variable resolutions (new version).
// =======================================================================

#ifndef __TILEMGR2_H
#define __TILEMGR2_H

//#define STRICT 1
#include "D3D7Client.h"
#include "D3D7Util.h"
#include "VPlanet.h"
#include "spherepatch.h"
#include "qtree.h"
#include "ztreemgr.h"

#define MAXQUEUE2 20

#define TILE_VALID  0x0001
#define TILE_ACTIVE 0x0002

#define TILE_FILERES 256
#define TILE_ELEVSTRIDE (TILE_FILERES+3)

// =======================================================================
// Type definitions

typedef struct {
	float tumin, tumax;
	float tvmin, tvmax;
} TEXCRDRANGE2;

// =======================================================================

class Tile {
	friend class TileManager2Base;
	friend class TileLoader;

public:
	Tile (TileManager2Base *_mgr, int _lvl, int _ilat, int _ilng);
	~Tile ();

	inline int Level() const { return lvl; }

	bool PreDelete();
	// Prepare tile for deletion. Return false if tile is locked

	bool InView (const MATRIX4 &transform);
	// tile in view of camera, given by transformation matrix 'transform'?

	void Extents (double *latmin, double *latmax, double *lngmin, double *lngmax) const;
	// Return the latitude/longitude extents of the tile

	virtual void MatchEdges () {}
	// Match edges with neighbour tiles

	inline const TEXCRDRANGE2 *GetTexRange () const { return &texrange; }
	// Returns the tile's texture coordinate range

	bool GetParentSubTexRange (TEXCRDRANGE2 *subrange);
	// Returns the texture range that allows to access the appropriate subregion of the
	// parent's texture

	inline LPDIRECTDRAWSURFACE7 Tex() { return tex; }
	inline const LPDIRECTDRAWSURFACE7 Tex() const { return tex; }

	enum TileState {
		Invalid   = 0x0000,                            // tile data have not been loaded/created yet
		InQueue   = 0x0004,                            // queued for asynchronous load
		Loading   = 0x0008,                            // in the process of being loaded
		Inactive  = TILE_VALID,                        // valid data, but not in active part of quadtree (cached)
		Active    = TILE_VALID | TILE_ACTIVE,          // active, but not rendered itself (ancestor of rendered tiles)
		Invisible = TILE_VALID | TILE_ACTIVE | 0x0004, // active, but outside field of view
		ForRender = TILE_VALID | TILE_ACTIVE | 0x0008  // rendered tile
	};

protected:
	virtual Tile *getParent() = 0;

	virtual void Load () = 0;
	// Load tile data from file, or synthesise missing data from ancestors

	virtual void Render () {}

	VECTOR3 Centre () const;
	// Returns the direction of the tile centre from the planet centre in local planet coordinates

	VBMESH *CreateMesh_quadpatch (int grdlat, int grdlng, INT16 *elev=0, double elev_scale = 1.0, double globelev=0.0, 
		const TEXCRDRANGE2 *range=0, bool shift_origin=false, VECTOR3 *shift=0, double bb_excess=0.0);
	// Creates a quadrilateral patch mesh

	VBMESH *CreateMesh_hemisphere (int grd, INT16 *elev=0, double globelev=0.0);
	// Creates a hemisphere mesh for eastern or western hemisphere at resolution level 4

	TileManager2Base *mgr;     // the manager this tile is associated with
	int lvl;                   // tile resolution level
	int ilat;                  // latitude index
	int ilng;                  // longitude index
	LPDIRECTDRAWSURFACE7 tex;  // diffuse surface texture
	bool owntex;               // true: tile owns the texture, false: tile uses ancestor subtexture
	TEXCRDRANGE2 texrange;     // texture coordinate subrange (if using ancestor subtexture)
	VBMESH *mesh;              // vertex-buffered tile mesh
	VECTOR3 cnt;               // tile centre in local planet coords
	VECTOR3 vtxshift;          // tile frame shift of origin from planet centre
	bool edgeok;               // edges have been checked in this frame
	TileState state;           // tile load/active/render state flags
	int lngnbr_lvl, latnbr_lvl, dianbr_lvl; // neighbour levels to which edges have been adapted
	mutable double mean_elev;  // mean tile elevation [m]
};

// =======================================================================

class TileLoader {
	template<class T> friend class TileManager2;

public:
	TileLoader (const oapi::D3D7Client *gclient);
	~TileLoader ();
	bool LoadTileAsync (Tile *tile);

	bool Unqueue (Tile *tile);
	// remove a tile from the load queue (caller must own hLoadMutex)

	inline static DWORD WaitForMutex() { return ::WaitForSingleObject (hLoadMutex, INFINITE); }
	inline static BOOL ReleaseMutex() { return ::ReleaseMutex (hLoadMutex); }

private:
	static struct QUEUEDESC {
		Tile *tile;
	} queue[MAXQUEUE2];

	static const oapi::D3D7Client *gc;
	static bool bRunThread;
	static int nqueue, queue_in, queue_out;
	HANDLE hLoadThread;
	static HANDLE hLoadMutex;
	static DWORD WINAPI Load_ThreadProc (void*);
	int load_frequency;
};

// =======================================================================

class TileManager2Base {
	friend class Tile;

public:
	struct configPrm {				// global configuration parameters
		int elevMode;                   // elevation mode (0=none, 1=linear, 2=cubic)
		bool bSpecular;					// render specular surface reflections?
		bool bLights;					// render planet night lights?
		bool bCloudShadow;				// render cloud shadows?
		double lightfac;				// city light brightness factor
		DWORD tileLoadFlags;            // 0x0001: load tiles from directory tree
		                                // 0x0002: load tiles from compressed archive
	};

	struct RenderPrm {
		const vPlanet::RenderPrm *rprm;  // render parameters inherited from the vPlanet object
		int maxlvl;        // max tile level
		MATRIX4 dwmat;     // planet world matrix, double precision
		MATRIX4 dwmat_tmp; // modifyable planet world matrix, double precision
		MATRIX4 dviewproj; // view+projection matrix, double precision
		MATRIX3 grot;      // planet rotation matrix
		VECTOR3 cpos;      // planet offset vector (in global frame)
		VECTOR3 cdir;      // camera direction from planet centre (in planet frame)
		VECTOR3 sdir;      // sun direction in local planet coords
		double cdist;      // camera distance from planet centre (in units of planet radii)
		double viewap;     // aperture of surface cap visible from camera pos
		double scale;
		bool fog;
		bool tint;         // apply atmospheric tint?
		VECTOR3 atm_tint;  // atmospheric RGB surface tint at high atmosphere
	} prm;

	TileManager2Base (const vPlanet *vplanet, int _maxres, int _gridres);

	static void GlobalInit (oapi::D3D7Client *gclient);
	static void GlobalExit ();

	static LPDIRECT3DDEVICE7 Dev() { return dev; }
	static LPDIRECT3D7 D3d() { return d3d; }
	static DWORD MemCaps() { return vbMemCaps; }

	template<class TileType>
	QuadTreeNode<TileType> *FindNode (QuadTreeNode<TileType> root[2], int lvl, int ilng, int ilat);
	// Returns the node at the specified position, or 0 if it doesn't exist

	template<class TileType>
	void ProcessNode (QuadTreeNode<TileType> *node);

	template<class TileType>
	void RenderNode (QuadTreeNode<TileType> *node);

	template<class TileType>
	void RenderNodeLabels (QuadTreeNode<TileType> *node, oapi::Sketchpad *skp, oapi::Font **labelfont, int *fontidx);

	void SetRenderPrm (MATRIX4 &dwmat, double prerot, bool use_zbuf, const vPlanet::RenderPrm &rprm);

	inline const oapi::D3D7Client *GClient() const { return gc; }
	inline const vPlanet *GetPlanet() const { return vp; }
	inline const OBJHANDLE &Cbody() const { return obj; }
	inline const configPrm &Cprm() const { return cprm; }
	inline const char *CbodyName() const { return cbody_name; }
	inline const double CbodySize() const { return obj_size; }
	inline const ELEVHANDLE ElevMgr() const { return emgr; }
	inline const int GridRes() const { return gridRes; }
	inline const double ElevRes() const { return elevRes; }

	/// \brief Return the root directory containing the body's texture data (surface, elevation, mask, cloud tiles)
	inline const std::string& DataRootDir() const { return m_dataRootDir; }

protected:
	MATRIX4 WorldMatrix (int ilng, int nlng, int ilat, int nlat);
	void SetWorldMatrix (const MATRIX4 &W);

	template<class TileType>
	QuadTreeNode<TileType> *LoadChildNode (QuadTreeNode<TileType> *node, int idx);
	// loads one of the four subnodes of 'node', given by 'idx'

	static configPrm cprm;
	std::string m_dataRootDir;       // the root directory (usually ending in the cbody's name) for all tile data (textures, elevations, etc.)
	double obj_size;                 // planet radius
	static TileLoader *loader;

private:
	const vPlanet *vp;               // the planet visual
	OBJHANDLE obj;                   // the planet object
	char cbody_name[256];
	ELEVHANDLE emgr;                 // elevation data query handle
	Camera *camera;
	int gridRes;                     // mesh grid resolution. must be multiple of 2. Default: 64 for surfaces, 32 for clouds
	double elevRes;                  // target elevation resolution

	static oapi::D3D7Client *gc;
	static DWORD vbMemCaps;
	static double resolutionBias;
	static double resolutionScale;
	static bool bTileLoadThread;     // load tiles on separate thread
	static LPDIRECT3D7 d3d;          // D3D instance
	static LPDIRECT3DDEVICE7 dev;    // render device
};

// =======================================================================

template<class TileType>
class TileManager2: public TileManager2Base {
	friend class Tile;

public:
	TileManager2 (const vPlanet *vplanet, int _maxres, int _gridres);
	~TileManager2 ();

	void SetRenderPrm(MATRIX4 &dwmat, double prerot, bool use_zbuf, const vPlanet::RenderPrm &rprm);

	void Render (MATRIX4 &dwmat, bool use_zbuf, const vPlanet::RenderPrm &rprm);
	void RenderLabels (oapi::Sketchpad *skp, oapi::Font **labelfont, int *fontidx);
	void RenderFlatCloudShadows (MATRIX4 &dwmat, const vPlanet::RenderPrm &rprm);

	void CreateLabels();
	void DeleteLabels();
	void SetSubtreeLabels(QuadTreeNode<TileType> *node, bool activate);

	QuadTreeNode<TileType> *FindNode (int lvl, int ilng, int ilat)
	{ return TileManager2Base::FindNode<TileType> (tiletree, lvl, ilng, ilat); }
	// Returns the node at the specified position, or 0 if it doesn't exist

	inline TileType *GlobalTile (int lvl)
	{ return (lvl >= -3 && lvl < 0 ? globtile[lvl+3] : 0); }
	// Returns a low-res global tile

	int Coverage (double latmin, double latmax, double lngmin, double lngmax, int maxlvl, const Tile **tbuf, int nt) const;
	// fills tbuf with a list of tiles up to maxlvl currently covering the area latmin,latmax,lngmin,lngmax
	// nt is the length of tbuf. Return value is the number of tiles copied into tbuf.
	// If the number of covering tiles is larger than nt, return value is -1. In that case, no tiles are copied.
	// The tile pointers are only valid for the current render pass. They are not guaranteed to exist any more
	// after the next call to Render.

	inline ZTreeMgr *ZTreeManager(int i) { return treeMgr[i]; }

protected:
	TileType *globtile[3];              // full-sphere tiles for resolution levels 1-3
	QuadTreeNode<TileType> tiletree[2]; // quadtree roots for western and eastern hemisphere

	ZTreeMgr **treeMgr;  // access to tile layers in compressed archives
	int ntreeMgr;

	void CheckCoverage (const QuadTreeNode<TileType> *node, double latmin, double latmax, double lngmin, double lngmax, int maxlvl, const Tile **tbuf, int nt, int *nfound) const;

	void LoadZTrees();
};

#endif // !__TILEMGR2_H