# Copyright (c) Martin Schweiger
# Licensed under the MIT License

add_library(TrackIR SHARED
	TrackIR.cpp
	TrackIR.rc
	TrackIRconfig.cpp
	NPClientWraps.cpp
)

target_include_directories(TrackIR
	PUBLIC ${SPACEXPANSE_SOURCE_SDK_INCLUDE_DIR}
	PUBLIC ${MODULE_COMMON_DIR}
)

target_link_libraries(TrackIR
	${SPACEXPANSE_LIB}
	${SPACEXPANSE_SDK_LIB}
	UxTheme.lib
)

add_dependencies(TrackIR
	${SpaceXpanseTgt}
	SpaceXpansesdk
)

set_target_properties(TrackIR
	PROPERTIES
	FOLDER Modules
)

# Installation
install(TARGETS TrackIR
	RUNTIME
	DESTINATION ${SPACEXPANSE_INSTALL_PLUGIN_DIR}
)
