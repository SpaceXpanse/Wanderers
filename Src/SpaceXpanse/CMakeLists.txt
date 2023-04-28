# Copyright (c) Martin Schweiger
# Licensed under the MIT License

# SAFESEH linker flag must be turned off because the DX7 libraries don't support it'
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -SAFESEH:NO")

# Sources for all SpaceXpanse executable targets
set(common_src
# General source files
	Astro.cpp
	Camera.cpp
	cmdline.cpp
	Config.cpp
	console_ng.cpp
	Element.cpp
	elevmgr.cpp
	Help.cpp
	Input.cpp
	Keymap.cpp
	LightEmitter.cpp
	Mesh.cpp
	Nav.cpp
	SpaceXpanse.cpp
	PlaybackEd.cpp
	Psys.cpp
	Script.cpp
	Shadow.cpp
	State.cpp
	Vecmat.cpp
	VectorMap.cpp
    ConsoleManager.cpp
# Launchpad
	Launchpad.cpp
	LpadTab.cpp
	TabAbout.cpp
	TabExtra.cpp
	TabJoystick.cpp
	TabModule.cpp
	TabParam.cpp
	TabScenario.cpp
	TabVideo.cpp
	TabVisual.cpp
# Dialogs
	CustomControls.cpp
	Dialogs.cpp
	DialogWin.cpp
	DlgCamera.cpp
	DlgCapture.cpp
	DlgFocus.cpp
	DlgFunction.cpp
	DlgHelp.cpp
	DlgInfo.cpp
	DlgMap.cpp
	DlgMenuCfg.cpp
	DlgMgr.cpp
	DlgRecorder.cpp
	DlgTacc.cpp
	DlgVishelper.cpp
	htmlctrl.c
	Select.cpp
# Body classes
	Body.cpp
	BodyIntegrator.cpp
	Celbody.cpp
	Planet.cpp
	Rigidbody.cpp
	Star.cpp
# Vessel classes
	FlightRecorder.cpp
	SuperVessel.cpp
	Vessel.cpp
	Vesselbase.cpp
	Vesselstatus.cpp
# Surface base classes
	Base.cpp
	Baseobj.cpp
# Cockpit classes
	Defpanel.cpp
	hud.cpp
	MenuInfoBar.cpp
	Pane.cpp
	Panel.cpp
	Panel2D.cpp
	VCockpit.cpp
# MFD classes
	Mfd.cpp
	MfdAlign.cpp
	MfdComms.cpp
	MfdDocking.cpp
	MfdHsi.cpp
	MfdLanding.cpp
	MfdMap.cpp
	MfdMap_old.cpp
	MfdOrbit.cpp
	MfdSurface.cpp
	MfdSync.cpp
	MfdTransfer.cpp
	MfdUser.cpp
# API implementations
	CamAPI.cpp
	DrawAPI.cpp
	GraphicsAPI.cpp
	MFDAPI.cpp
	ModuleAPI.cpp
	SpaceXpanseAPI.cpp
# Graphics utils
	D3d7util.cpp
	D3dmath.cpp
	Di7frame.cpp
# Graphics interface base class for GDI clients
	${GDICLIENT_DIR}/GDIClient.cpp
# Utils
	Log.cpp
	Memstat.cpp
	Util.cpp
	ZTreeMgr.cpp
# Resources
	SpaceXpanse.rc
	SpaceXpanse.rc2
	SpaceXpanse.ico
# Files built on the fly
	cryptstring.h
	Disclaimer.txt
	splash_cs.dat
)

# Sources for DX7 built-in graphics version only
set(graphics_src
# DX7 inline graphics
	cloudmgr2.cpp
	CSphereMgr.cpp
	cspheremgr2.cpp
	D3d7enum.cpp
	D3d7frame.cpp
	ExtraRender.cpp
	OGraphics.cpp
	Particle.cpp
	Scene.cpp
	ScreenNote.cpp
	Spherepatch.cpp
	surfmgr2.cpp
	Texture.cpp
	tilelabel.cpp
	TileMgr.cpp
	tilemgr2.cpp
	VBase.cpp
	Vobject.cpp
	VPlanet.cpp
	Vstar.cpp
	Vvessel.cpp
)

set(SpaceXpanse_includes
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/SpaceXpansesdk/include
	${CMAKE_SOURCE_DIR}/OVP
	${CMAKE_CURRENT_BINARY_DIR}
	${ZLIB_INCLUDE_DIR}
	${HTMLHELP_INCLUDE_DIR}
	${DX7SDK_INCLUDE_DIR}
)

set(SpaceXpanse_common_libs
	Winmm.dll
	comctl32.lib
	UxTheme.lib
	${HTML_HELP_LIBRARY}
	${ZLIB_LIB}
	${HTMLHELP_LIB}
	$<TARGET_FILE:SpaceXpansesdk>
	$<TARGET_FILE:DlgCtrl>
)
set(SpaceXpanse_dx7_libs
	dxguid.lib
	dinput8.lib
	${DX7SDK_LIB_DIR}/d3dim.lib
	${DX7SDK_LIB_DIR}/ddraw.lib
)
set(SpaceXpanse_server_libs
	dxguid.lib
	dinput8.lib
)

set(SpaceXpanse_depends
	scramble
	fchecksum
	SpaceXpansesdk
	DlgCtrl
)

# SpaceXpanse executable (DX7 inline graphics version)
if(BUILD_SPACEXPANSE_DX7)

	add_executable(SpaceXpanse
		${common_src}
		${graphics_src}
	)

	set_target_properties(SpaceXpanse
		PROPERTIES
		LINK_FLAGS "/SUBSYSTEM:WINDOWS"
		COMPILE_FLAGS "${CompileFlags}"
		RUNTIME_OUTPUT_DIRECTORY ${SPACEXPANSE_BINARY_ROOT_DIR}
		COMPILE_DEFINITIONS "INLINEGRAPHICS;DIRECTINPUT_VERSION=0x0800"
		ENABLE_EXPORTS 1
		FOLDER Core
	)

	target_include_directories(SpaceXpanse PUBLIC
		${SpaceXpanse_includes}
	)
	
	target_link_libraries(SpaceXpanse
		${SpaceXpanse_common_libs}
		${SpaceXpanse_dx7_libs}
	)

	add_dependencies(SpaceXpanse
		${SpaceXpanse_depends}
	)

	# Installation: SpaceXpanse executable (standalone graphics version)
	install(TARGETS
		SpaceXpanse
		RUNTIME
		DESTINATION ${SPACEXPANSE_INSTALL_ROOT_DIR}
	)

	# Installation: SpaceXpanse SDK interface library to SDK folder to be included by addons
	install(FILES $<TARGET_LINKER_FILE:SpaceXpanse>
		DESTINATION ${SPACEXPANSE_INSTALL_SDK_DIR}/lib
	)
	
endif()

# SpaceXpanse executable (graphics server version)
if(BUILD_SPACEXPANSE_SERVER)

	add_executable(SpaceXpanse_server
		${common_src}
	)

	set_target_properties(SpaceXpanse_server
		PROPERTIES
		LINK_FLAGS "/SUBSYSTEM:CONSOLE /ENTRY:WinMainCRTStartup"
		COMPILE_FLAGS "${CompileFlags}"
		RUNTIME_OUTPUT_DIRECTORY ${SPACEXPANSE_BINARY_MODULE_DIR}/Server
		OUTPUT_NAME SpaceXpanse
		VS_DEBUGGER_WORKING_DIRECTORY ${SPACEXPANSE_BINARY_ROOT_DIR}
		COMPILE_DEFINITIONS "NOGRAPHICS;DIRECTINPUT_VERSION=0x0800"
		FOLDER Core
	)

	target_include_directories(SpaceXpanse_server PUBLIC
		${SpaceXpanse_includes}
	)

	target_link_libraries(SpaceXpanse_server
		${SpaceXpanse_common_libs}
		${SpaceXpanse_server_libs}
	)

	add_dependencies(SpaceXpanse_server
		${SpaceXpanse_depends}
	)

	# Installation: SpaceXpanse executable (graphics-server versions)
	install(TARGETS
		SpaceXpanse_server
		RUNTIME
		DESTINATION ${SPACEXPANSE_INSTALL_MODULE_DIR}/Server
	)
	# If the standard SpaceXpanse executable is not built...
	if(NOT BUILD_SPACEXPANSE_DX7)
		set_target_properties(SpaceXpanse_server
			PROPERTIES
			ENABLE_EXPORTS 1
		)
		# ...we have to use the NG version to export the API links
		install(FILES $<TARGET_LINKER_FILE:SpaceXpanse_server>
			DESTINATION ${SPACEXPANSE_INSTALL_SDK_DIR}/lib
		)
		# ...and copy SpaceXpanse.exe into build dir (not install dir) so that tests work
		add_custom_command(TARGET SpaceXpanse_server
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SpaceXpanse_server> ${SPACEXPANSE_BINARY_ROOT_DIR}
		)
	endif()
	
endif()

# Copy library to its SpaceXpansesdk/lib location so that external project integrated into the build can find it
add_custom_command(TARGET ${SpaceXpanseTgt}
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E make_directory ${SPACEXPANSE_BINARY_SDK_DIR}/lib
	COMMAND ${CMAKE_COMMAND} -E copy ${SPACEXPANSE_LIB} ${SPACEXPANSE_BINARY_SDK_DIR}/lib/ 
)

add_custom_command(
	OUTPUT cryptstring.h
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/cryptstring.h.in
	COMMAND $<TARGET_FILE:scramble> ${CMAKE_CURRENT_SOURCE_DIR}/cryptstring.h.in cryptstring.h
)

add_custom_command(
	OUTPUT Disclaimer.txt
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Disclaimer.txt.in
	COMMAND $<TARGET_FILE:scramble> ${CMAKE_CURRENT_SOURCE_DIR}/Disclaimer.txt.in Disclaimer.txt
)

add_custom_command(
	OUTPUT splash_cs.dat
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Bitmaps/Splash.jpg
	COMMAND $<TARGET_FILE:fchecksum> ${CMAKE_CURRENT_SOURCE_DIR}/Bitmaps/Splash.jpg splash_cs.dat
)
