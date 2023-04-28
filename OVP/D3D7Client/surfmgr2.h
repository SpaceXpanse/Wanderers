// Copyright (c) Martin Schweiger
// Licensed under the MIT License

// ==============================================================
//   SPACEXPANSE VISUALISATION PROJECT (OVP)
//   D3D7 Client module
// ==============================================================

// ==============================================================
// surfmgr2.h
// Planetary surface rendering engine v2, including a simple
// LOD (level-of-detail) algorithm for surface patch resolution.
// ==============================================================

#ifndef __SURFMGR2_H
#define __SURFMGR2_H

#include "tilemgr2_imp.hpp"

class SurfTile: public Tile {
	friend class TileManager2Base;
	template<class SurfTile> friend class TileManager2;
	friend class TileLabel;

	void MatchEdges ();

public:
	SurfTile (TileManager2Base *_mgr, int _lvl, int _ilat, int _ilng);
	~SurfTile ();

	inline void SetNode (QuadTreeNode<SurfTile> *_node) { node = _node; }
	// Register the tile to a quad tree node

protected:
	virtual Tile *getParent() { return node && node->Parent() ? node->Parent()->Entry() : NULL; }
	inline SurfTile *getSurfParent() { return node && node->Parent() ? node->Parent()->Entry() : NULL; }
	// Return pointer to parent tile, if exists

	void Load ();
	INT16 *ReadElevationFile (int lvl, int ilat, int ilng, double tgt_res, double *mean_elev=0);
	bool LoadElevationData ();
	void Render ();
	void RenderLabels (oapi::Sketchpad *skp, oapi::Font **labelfont, int *fontidx);

	TileManager2<SurfTile> *smgr;	// surface tile manager interface
	QuadTreeNode<SurfTile> *node;	// my node in the quad tree, if I'm part of a tree

	void FixCorner (const SurfTile *nbr);
	// Match corner elevation to neighbour

	void FixLongitudeBoundary (const SurfTile *nbr, bool keep_corner=false);
	// Match longitude edge elevation to neighbour. If keep_corner==true, skip corner node

	void FixLatitudeBoundary (const SurfTile *nbr, bool keep_corner=false);
	// Match latitude edge elevation to neighbour. If keep_corner==true, skip corner node

	void CreateLabels();
	// create the label object from the label tile file, if available

	inline void DeleteLabels() { if (label) { delete label; label = 0; } }
	// delete the TileLabel object if it exists

private:
	INT16 *ElevationData () const;
	double GetMeanElevation (const INT16 *elev) const;

	LPDIRECTDRAWSURFACE7 ltex;	// landmask/nightlight texture, if applicable
	INT16 *elev;				// elevation data [m] (8x subsampled)
	mutable INT16 *ggelev;		// pointer to my elevation data in the great-grandparent
	bool has_elevfile;			// true if the elevation data for this tile were read from file

	TileLabel *label;			// surface labels associated with this tile
};

#endif // !__SURFMGR2_H