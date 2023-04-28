# Copyright (c) Martin Schweiger
# Licensed under the MIT License

add_library(Framerate SHARED
	Framerate.cpp
	Framerate.rc
	../Common/Dialog/Graph.cpp
	../Common/Dialog/TabDlg.cpp
)

target_include_directories(Framerate
	PUBLIC ${SPACEXPANSE_SOURCE_SDK_INCLUDE_DIR}
	PUBLIC ${MODULE_COMMON_DIR}
)

target_link_libraries(Framerate
	${SPACEXPANSE_LIB}
	${SPACEXPANSE_SDK_LIB}
	UxTheme.lib
)

add_dependencies(Framerate
	${SpaceXpanseTgt}
	SpaceXpansesdk
)

set_target_properties(Framerate
	PROPERTIES
	FOLDER Modules
)

# Installation
install(TARGETS Framerate
	RUNTIME
	DESTINATION ${SPACEXPANSE_INSTALL_PLUGIN_DIR}
)
