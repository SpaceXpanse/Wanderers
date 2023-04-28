// ==============================================================
// Part of the SPACEXPANSE VISUALISATION PROJECT (OVP)
// Dual licensed under GPL v3 and LGPL v3
// Copyright (C) 2014-2016 Jarmo Nikkanen
// ==============================================================

#ifndef __PLANETRENDERER_H
#define __PLANETRENDERER_H

#include "D3D9Client.h"

/**
 * \brief Global planet renderer object
 *
 * The global planet renderer object provides objects and handles
 * (like D3DX Tech HANDLEs) usefull to all rendering instances.
 */
class PlanetRenderer {

public:
	/**
	 * \brief Constructs a new planet renderer object
	 */
	PlanetRenderer();

	/**
	 * \brief Destroys the planet renderer object
	 */
	~PlanetRenderer();

	/**
	 * \brief Set up global parameters shared by all instances
	 * \param gclient client instance pointer
	 */
	static void GlobalInit(class oapi::D3D9Client *gclient);

	/**
	 * \brief Release global parameters
	 */
	static void GlobalExit();
	static void	InitializeScattering(class vPlanet *pPlanet);

	static LPDIRECT3DDEVICE9 Dev() { return pDev; }
	static ID3DXEffect * Shader() { return pShader; }
	static oapi::D3D9Client * Client() { return gc; }

	static void	SetWorldMatrix (const D3DXMATRIX &W);
	static void	SetViewProjectionMatrix (const D3DXMATRIX *VP);

	// ------------------------------------------------------------
	static oapi::D3D9Client *gc;
	static LPDIRECT3DDEVICE9 pDev;
	static ID3DXEffect *pShader;
	static LPDIRECT3DTEXTURE9 hOcean;
	static LPDIRECT3DTEXTURE9 hCloudMicro;
	static LPDIRECT3DTEXTURE9 hCloudMicroNorm;
	static VECTOR3 vLPosOld;
	static bool bEnvMapEnabled;
	// ------------------------------------------------------------
	static D3DXHANDLE eTileTech;
	static D3DXHANDLE eCloudTech;
	static D3DXHANDLE eRingTech;
	static D3DXHANDLE eHorizonTech;
	static D3DXHANDLE eSkyDomeTech;
	// ------------------------------------------------------------
	static D3DXHANDLE ssLight;
	// ------------------------------------------------------------
	static D3DXHANDLE smWorld;
	static D3DXHANDLE smViewProj;
	static D3DXHANDLE smLVP;
	// ------------------------------------------------------------
	static D3DXHANDLE svTexOff;
	static D3DXHANDLE svWater;
	static D3DXHANDLE svSunDir;
	static D3DXHANDLE svCloudOff;
	static D3DXHANDLE svMicroOff;
	static D3DXHANDLE svOverlayOff;
	static D3DXHANDLE svOverlayCtrl;
	static D3DXHANDLE svTangent;
	static D3DXHANDLE svBiTangent;
	static D3DXHANDLE svPolarAxis;
	static D3DXHANDLE svMicroScale0;
	static D3DXHANDLE svMicroScale1;
	static D3DXHANDLE svMicroScale2;
	static D3DXHANDLE svSHD;
	// ------------------------------------------------------------
	static D3DXHANDLE sfDistScale;
	static D3DXHANDLE sfAlpha;
	static D3DXHANDLE sfNight;
	// ------------------------------------------------------------
	static D3DXHANDLE sbCloudSh;
	static D3DXHANDLE sbLights;
	static D3DXHANDLE sbLocals;
	static D3DXHANDLE sbInSpace;
	static D3DXHANDLE sbOnOff;
	static D3DXHANDLE sbEnvEnable;
	static D3DXHANDLE sbMicroNormals;
	static D3DXHANDLE siTileLvl;
	static D3DXHANDLE siDebug;
	static D3DXHANDLE sbDebug;
	static D3DXHANDLE sbShadows;
	static D3DXHANDLE sbOverlay;
	static D3DXHANDLE sbElevOvrl;
	static D3DXHANDLE sbSpherical;
	static D3DXHANDLE sbCloudNorm;
	static D3DXHANDLE sbEarth;
	// ------------------------------------------------------------
	static D3DXHANDLE stDiff;
	static D3DXHANDLE stMask;
	static D3DXHANDLE stCloud;
	static D3DXHANDLE stCloud2;
	static D3DXHANDLE stCloudMicro;
	static D3DXHANDLE stCloudMicroNorm;
	static D3DXHANDLE stNoise;
	static D3DXHANDLE stOcean;
	static D3DXHANDLE stEnvMap;
	static D3DXHANDLE stMicroA;
	static D3DXHANDLE stMicroB;
	static D3DXHANDLE stMicroC;
	static D3DXHANDLE stMicroRot;
	static D3DXHANDLE stShadowMap;
	static D3DXHANDLE stOverlay;
	static D3DXHANDLE stMskOverlay;
	static D3DXHANDLE stElvOverlay;
	// ------------------------------------------------------------
	static D3DXHANDLE sfGlobalAmb;
	static D3DXHANDLE sfAmbient0;
	// Scatter model ----------------------------------------------
	static D3DXHANDLE svMPhase;
	static D3DXHANDLE svODCoEff;
	static D3DXHANDLE svODCoEffEx;
	static D3DXHANDLE svMieInSct;
	static D3DXHANDLE svRayInSct;
	static D3DXHANDLE svTotOutSct;
	static D3DXHANDLE svColorShift;
	static D3DXHANDLE svHazeMax;
	static D3DXHANDLE svCameraPos;
	static D3DXHANDLE svUnitCameraPos;
	static D3DXHANDLE sfCloudInts;
	static D3DXHANDLE sfSunAlt;
	static D3DXHANDLE sfScaleHeight;
	static D3DXHANDLE sfInvScaleHeight;
	static D3DXHANDLE sfRadius;
	static D3DXHANDLE sfCameraAlt;
	static D3DXHANDLE sfHorizonAlt;
	static D3DXHANDLE sfAtmRad2;
	static D3DXHANDLE sfRPhase;
	static D3DXHANDLE sfHorizonDst;
	static D3DXHANDLE sfExposure;
	static D3DXHANDLE sfAux1;
	static D3DXHANDLE sfAux2;
	static D3DXHANDLE sfTrGamma;
	static D3DXHANDLE sfAtmGamma;
	static D3DXHANDLE sfInvAux1;
	static D3DXHANDLE sfInvParameter;
	static D3DXHANDLE sfTime;
};

#endif
