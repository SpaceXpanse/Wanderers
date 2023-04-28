// ==============================================================
// Configuration Manager
// Part of the SPACEXPANSE VISUALISATION PROJECT (OVP)
// Dual licensed under GPL v3 and LGPL v3
// Copyright (C) 2007-2016 Martin Schweiger
//				 2011-2016 Jarmo Nikkanen
// ==============================================================

#include "D3D9Config.h"
#include "SpaceXpansesdk.h"

static char *cfgfile = "D3D9Client.cfg";

class D3D9Config *Config;				// configuration manager

// ==============================================================

D3D9Config::D3D9Config()
{
	Reset();
	ReadParams();
}

D3D9Config::~D3D9Config ()
{
	WriteParams ();
	delete []DebugFont;
	DebugFont = NULL;
	delete []SolCfg;
	SolCfg = NULL;
}

void D3D9Config::Reset ()
{
	OrbitalShadowMult   = 0.85;
	PlanetPreloadMode	= 0;
	PlanetLoadFrequency	= 20;
	Anisotrophy			= 4;
	SceneAntialias		= 4;
	DebugLvl			= 1;
	VCNearPlane			= 0.1;
	LightConfig			= 0;
	NearClipPlane		= 0;
	NVPerfHUD			= 0;
	PreLBaseVis			= 0;
	DebugFontSize		= 18;
	UseNormalMap		= 1;
	SketchpadFont		= 1;
	RwyLightAnimate		= 1;
	RwyLightAngle		= 120.0;
	RwyBrightness		= 1.0;
	Convergence			= 0.2;
	Separation			= 65.0;
	SunAngle			= 10.0;
	BumpAmp				= 1.0;
	PlanetGlow			= 1.0;
	EnvMapSize			= 256;
	EnvMapMode			= 0;
	EnvMapFaces			= 1;
	EnableGlass			= 1;
	EnableMeshDbg		= 1;
	ShadowMapMode		= 1;
	ShadowFilter		= 0;
	ShadowMapSize		= 2048;
	FrameRate			= 200.0;
	EnableLimiter		= 0;
	CustomCamMode		= 1;
	TileMipmaps			= 0;
	TextureMips			= 1;
	LODBias				= 0.0;
	MeshRes				= 1;
	TileDebug			= 0;
	MicroMode			= 1;
	MicroFilter			= 2;
	BlendMode			= 1;
	MicroBias			= 3;
	PostProcess			= 0;
	ShaderDebug			= 0;
	PresentLocation		= 1;
	PlanetTileLoadFlags	= 0x3;
	TerrainShadowing	= 1;
	LabelDisplayFlags	= LABEL_DISPLAY_RECORD | LABEL_DISPLAY_REPLAY;
	CloudMicro			= 1;
	GDIOverlay			= 0;
	gcGUIMode			= 0;
	bAbsAnims			= 0;
	bCloudNormals		= 1;
	bFlats				= 0;
	DebugBreak			= 0;
	ShaderCacheUse		= 1;

	GFXIntensity = 0.5;
	GFXDistance = 0.8;
	GFXThreshold = 1.1;
	GFXGamma = 1.0;
	GFXSunIntensity = 1.2;
	GFXLocalMax = 0.5;

	DisableDriverManagement = 0;
	DisableVisualHelperReadout = 0;

	SolCfg				= new char[64];   strcpy_s(SolCfg,64,"Sol");
	DebugFont			= new char[64];   strcpy_s(DebugFont,64,"Fixed");
}

int D3D9Config::MaxLights()
{
	if (LightConfig == 0) return 1;
	if (LightConfig <= 2) return 4;
	return 8;
}

bool D3D9Config::ReadParams ()
{
	int i;
	double d;

	FILEHANDLE hFile = oapiOpenFile(cfgfile, FILE_IN_ZEROONFAIL, ROOT);
	if (!hFile) return false;

	if (oapiReadItem_float (hFile, "FrameRate", d))				FrameRate = max(0.0, min(300.0, d));
	if (oapiReadItem_int   (hFile, "EnableLimiter", i))			EnableLimiter = max(0, min(1, i));
	if (oapiReadItem_int   (hFile, "CustomCamMode", i))			CustomCamMode = max(0, min(1, i));
	if (oapiReadItem_int   (hFile, "PlanetPreloadMode", i))		PlanetPreloadMode = max(0, min(1, i));
	if (oapiReadItem_int   (hFile, "PlanetTexLoadFreq", i))		PlanetLoadFrequency = max(1, min(1000, i));
	if (oapiReadItem_int   (hFile, "Anisotrophy", i))			Anisotrophy = max(1, min(16, i));
	if (oapiReadItem_int   (hFile, "SceneAntialias", i))		SceneAntialias = i;
	if (oapiReadItem_int   (hFile, "SketchpadFont", i))			SketchpadFont = max(0, min(2, i));
	if (oapiReadItem_int   (hFile, "PreLoadBaseVisuals", i))	PreLBaseVis = max(0, min(1, i));
	if (oapiReadItem_int   (hFile, "EnableNormalMapping", i))	UseNormalMap = max(0, min(1, i));
	if (oapiReadItem_int   (hFile, "NearClipPlaneMode", i))		NearClipPlane = max(0, min(1, i));
	if (oapiReadItem_int   (hFile, "RwyLightAnimate", i))		RwyLightAnimate = max(0, min(1, i));
	if (oapiReadItem_float (hFile, "RwyLightAngle", d))			RwyLightAngle = max(10.0, min(180.0, d));
	if (oapiReadItem_float (hFile, "RwyBrightness", d))			RwyBrightness = max(0.3, min(3.0, d));
	if (oapiReadItem_float (hFile, "NightLightsAngle", d))		SunAngle = max(0.1, min(20.0, d));
	if (oapiReadItem_float (hFile, "BumpMapAmplitude", d))		BumpAmp = max(0.1, min(10.0, d));
	if (oapiReadItem_float (hFile, "PlanetGlow", d))			PlanetGlow = max(0.01, min(2.5, d));
	if (oapiReadItem_int   (hFile, "EnvMapSize", i))			EnvMapSize = max(64, min(512, i));
	if (oapiReadItem_int   (hFile, "EnvMapMode", i))			EnvMapMode = max(0, min(2, i));
	if (oapiReadItem_int   (hFile, "EnvMapFaces", i))			EnvMapFaces = max(1, min(3, i));
	if (oapiReadItem_int   (hFile, "ShadowMapMode", i))			ShadowMapMode = max(0, min(3, i));
	if (oapiReadItem_int   (hFile, "ShadowMapFilter", i))	 	ShadowFilter = max(0, min(5, i));
	if (oapiReadItem_int   (hFile, "ShadowMapSize", i))			ShadowMapSize = max(512, min(4096, i));
	if (oapiReadItem_int   (hFile, "EnableGlass", i))			EnableGlass = max(0, min(1, i));
	if (oapiReadItem_int   (hFile, "TerrainShadowing", i))		TerrainShadowing = max(0, min(2, i));
	if (oapiReadItem_int   (hFile, "EnableMeshDbg", i))			EnableMeshDbg = max(0, min(1, i));
	if (oapiReadItem_int   (hFile, "TileMipmaps", i))			TileMipmaps = max(0, min(2, i));
	if (oapiReadItem_int   (hFile, "TextureMips", i))			TextureMips = max(0, min(2, i));
	if (oapiReadItem_int   (hFile, "TileDebug", i))				TileDebug = max(0, min(1, i));
	if (oapiReadItem_float (hFile, "StereoSeparation", d))		Separation = max(10.0, min(100.0, d));
	if (oapiReadItem_float (hFile, "StereoConvergence", d))		Convergence = max(0.05, min(1.0, d));
	if (oapiReadItem_int   (hFile, "DebugLvl", i))				DebugLvl = i;
	if (oapiReadItem_float (hFile, "VCNearPlane", d))			VCNearPlane = max(-1.0, min(1.0, d));
	if (oapiReadItem_int   (hFile, "LightCongiguration", i))	LightConfig = max(min(4, i), 0); // Old typo stored?
	if (oapiReadItem_int   (hFile, "LightConfiguration", i))	LightConfig = max(min(4, i), 0); // ...this will override it anyhow
	if (oapiReadItem_int   (hFile, "DisableDrvMgm", i))			DisableDriverManagement = max(0, min(1, i));
	if (oapiReadItem_int   (hFile, "NVPerfHUD", i))				NVPerfHUD = max(0, min(1, i));
	if (oapiReadItem_int   (hFile, "DebugLineFontSize", i))		DebugFontSize = i;
	if (oapiReadItem_int   (hFile, "DisableVisualHelperReadout", i))	DisableVisualHelperReadout = max(0, min(1, i));
	if (oapiReadItem_float (hFile, "LODBias", d))						LODBias = max(-2.0, min(2.0, d));
	if (oapiReadItem_int   (hFile, "MeshRes", i))						MeshRes = max(0, min(2, i));
	if (oapiReadItem_int   (hFile, "MicroMode", i))						MicroMode = max(0, min(1, i));
	if (oapiReadItem_int   (hFile, "MicroFilter", i))					MicroFilter = max(0, min(5, i));
	if (oapiReadItem_int   (hFile, "BlendMode", i))						BlendMode = max(0, min(2, i));
	if (oapiReadItem_int   (hFile, "MicroBias", i))						MicroBias = max(0, min(10, i));
	if (oapiReadItem_int   (hFile, "CloudMicro", i))					CloudMicro = max(0, min(1, i));
	if (oapiReadItem_int   (hFile, "PostProcess", i))					PostProcess = max(0, min(2, i));
	if (oapiReadItem_int   (hFile, "ShaderDebug", i))					ShaderDebug = max(0, min(1, i));
	if (oapiReadItem_int   (hFile, "PresentLocation", i))				PresentLocation = max(0, min(1, i));
	if (oapiReadItem_int   (hFile, "PlanetTileLoadFlags", i))			PlanetTileLoadFlags = max(1, min(3, i));
	if (oapiReadItem_int   (hFile, "LabelDisplayFlags", i))				LabelDisplayFlags = max(0, min(3, i));
	if (oapiReadItem_int   (hFile, "GDIOverlay", i))					GDIOverlay = max(0, min(1, i));
	if (oapiReadItem_int   (hFile, "gcGUIMode", i))						gcGUIMode = max(0, min(3, i));
	if (oapiReadItem_int   (hFile, "AbsoluteAnimations", i))			bAbsAnims = max(0, min(1, i));
	if (oapiReadItem_int   (hFile, "NormalmappedClouds", i))			bCloudNormals = max(0, min(1, i));
	if (oapiReadItem_int   (hFile, "TerrainFlats", i))					bFlats = max(0, min(1, i));
	if (oapiReadItem_int   (hFile, "DebugBreak", i))					DebugBreak = max(0, min(1, i));
	if (oapiReadItem_int   (hFile, "ShaderCacheUse", i))				ShaderCacheUse = max(0, min(1, i));
	if (oapiReadItem_float (hFile, "OrbitalShadowMult", d))			    OrbitalShadowMult = max(0.5, min(10.0, d));

	if (oapiReadItem_float (hFile, "GFXIntensity", d))					GFXIntensity = max(0.0, min(1.0, d));
	if (oapiReadItem_float (hFile, "GFXDistance", d))					GFXDistance = max(0.0, min(1.0, d));
	if (oapiReadItem_float (hFile, "GFXThreshold", d))					GFXThreshold = max(0.5, min(2.0, d));
	if (oapiReadItem_float (hFile, "GFXGamma", d))						GFXGamma = max(0.3, min(2.5, d));
	if (oapiReadItem_float (hFile, "GFXSunIntensity", d))				GFXSunIntensity = max(0.5, min(2.5, d));
	if (oapiReadItem_float (hFile, "GFXLocalMax", d))					GFXLocalMax = max(0.001, min(1.0, d));


	oapiReadItem_string (hFile, "SolCfg", SolCfg);
	oapiReadItem_string (hFile, "DebugLineFont", DebugFont);

	oapiCloseFile (hFile, FILE_IN_ZEROONFAIL);

	return true;
}


void D3D9Config::WriteParams ()
{
	FILEHANDLE hFile = oapiOpenFile (cfgfile, FILE_OUT, ROOT);

	oapiWriteItem_float (hFile, "FrameRate", FrameRate);
	oapiWriteItem_int   (hFile, "EnableLimiter", EnableLimiter);
	oapiWriteItem_int   (hFile, "CustomCamMode", CustomCamMode);
	oapiWriteItem_int   (hFile, "PlanetPreloadMode", PlanetPreloadMode);
	oapiWriteItem_int   (hFile, "PlanetTexLoadFreq", PlanetLoadFrequency);
	oapiWriteItem_int   (hFile, "Anisotrophy", Anisotrophy);
	oapiWriteItem_int   (hFile, "SceneAntialias", SceneAntialias);
	oapiWriteItem_int   (hFile, "SketchpadFont", SketchpadFont);
	oapiWriteItem_int   (hFile, "PreLoadBaseVisuals", PreLBaseVis);
	oapiWriteItem_int   (hFile, "EnableNormalMapping", UseNormalMap);
	oapiWriteItem_int   (hFile, "NearClipPlaneMode", NearClipPlane);
	oapiWriteItem_int   (hFile, "RwyLightAnimate", RwyLightAnimate);
	oapiWriteItem_float (hFile, "RwyLightAngle", RwyLightAngle);
	oapiWriteItem_float (hFile, "RwyBrightness", RwyBrightness);
	oapiWriteItem_float (hFile, "NightLightsAngle", SunAngle);
	oapiWriteItem_float (hFile, "BumpMapAmplitude", BumpAmp);
	oapiWriteItem_float (hFile, "PlanetGlow", PlanetGlow);
	oapiWriteItem_int   (hFile, "EnvMapSize", EnvMapSize);
	oapiWriteItem_int   (hFile, "EnvMapMode", EnvMapMode);
	oapiWriteItem_int   (hFile, "EnvMapFaces", EnvMapFaces);
	oapiWriteItem_int   (hFile, "ShadowMapMode", ShadowMapMode);
	oapiWriteItem_int   (hFile, "ShadowMapFilter", ShadowFilter);
	oapiWriteItem_int   (hFile, "ShadowMapSize", ShadowMapSize);
	oapiWriteItem_int   (hFile, "TerrainShadowing", TerrainShadowing);
	oapiWriteItem_int   (hFile, "EnableGlass", EnableGlass);
	oapiWriteItem_int   (hFile, "EnableMeshDbg", EnableMeshDbg);
	oapiWriteItem_int   (hFile, "TileMipmaps", TileMipmaps);
	oapiWriteItem_int   (hFile, "TextureMips", TextureMips);
	oapiWriteItem_int   (hFile, "TileDebug", TileDebug);
	oapiWriteItem_float (hFile, "StereoSeparation", Separation);
	oapiWriteItem_float (hFile, "StereoConvergence", Convergence);
	oapiWriteItem_int   (hFile, "DebugLvl", DebugLvl);
	oapiWriteItem_float (hFile, "VCNearPlane", VCNearPlane);
	oapiWriteItem_int   (hFile, "LightConfiguration", LightConfig);
	oapiWriteItem_int   (hFile, "DisableDrvMgm", DisableDriverManagement);
	oapiWriteItem_int   (hFile, "NVPerfHUD", NVPerfHUD);
	oapiWriteItem_int   (hFile, "DebugLineFontSize", DebugFontSize);
	oapiWriteItem_int   (hFile, "DisableVisualHelperReadout", DisableVisualHelperReadout);
	oapiWriteItem_float (hFile, "LODBias", LODBias);
	oapiWriteItem_int   (hFile, "MeshRes", MeshRes);
	oapiWriteItem_int   (hFile, "MicroMode", MicroMode);
	oapiWriteItem_int   (hFile, "MicroFilter", MicroFilter);
	oapiWriteItem_int   (hFile, "BlendMode", BlendMode);
	oapiWriteItem_int   (hFile, "MicroBias", MicroBias);
	oapiWriteItem_int	(hFile, "CloudMicro", CloudMicro);
	oapiWriteItem_int   (hFile, "PostProcess", PostProcess);
	oapiWriteItem_int   (hFile, "ShaderDebug", ShaderDebug);
	oapiWriteItem_int   (hFile, "PresentLocation", PresentLocation);
	oapiWriteItem_int   (hFile, "PlanetTileLoadFlags", PlanetTileLoadFlags);
	oapiWriteItem_int   (hFile, "LabelDisplayFlags", LabelDisplayFlags);
	oapiWriteItem_int   (hFile, "GDIOverlay", GDIOverlay);
	oapiWriteItem_int	(hFile, "gcGUIMode", gcGUIMode);
	oapiWriteItem_int   (hFile, "AbsoluteAnimations", bAbsAnims);
	oapiWriteItem_int   (hFile, "NormalmappedClouds", bCloudNormals);
	oapiWriteItem_int   (hFile, "TerrainFlats", bFlats);
	oapiWriteItem_int	(hFile, "DebugBreak", DebugBreak);
	oapiWriteItem_int	(hFile, "ShaderCacheUse", ShaderCacheUse);
	oapiWriteItem_float (hFile, "OrbitalShadowMult", OrbitalShadowMult);

	oapiWriteItem_float (hFile, "GFXIntensity", GFXIntensity);
	oapiWriteItem_float (hFile, "GFXDistance", GFXDistance);
	oapiWriteItem_float (hFile, "GFXThreshold", GFXThreshold);
	oapiWriteItem_float (hFile, "GFXGamma", GFXGamma);
	oapiWriteItem_float (hFile, "GFXSunIntensity", GFXSunIntensity);
	oapiWriteItem_float (hFile, "GFXLocalMax", GFXLocalMax);

	oapiWriteItem_string (hFile, "SolCfg", SolCfg);
	oapiWriteItem_string (hFile, "DebugLineFont", DebugFont);

	oapiCloseFile (hFile, FILE_OUT);
}
