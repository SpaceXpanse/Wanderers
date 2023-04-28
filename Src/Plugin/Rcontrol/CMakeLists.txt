# Copyright (c) Martin Schweiger
# Licensed under the MIT License

add_library(Rcontrol SHARED
	Rcontrol.cpp
	Rcontrol.rc
)

target_include_directories(Rcontrol
	PUBLIC ${SPACEXPANSE_SOURCE_SDK_INCLUDE_DIR}
)

target_link_libraries(Rcontrol
	${SPACEXPANSE_LIB}
	${SPACEXPANSE_SDK_LIB}
	${SPACEXPANSE_DLGCTRL_LIB}
)

add_dependencies(Rcontrol
	${SpaceXpanseTgt}
	SpaceXpansesdk
	DlgCtrl
)

set_target_properties(Rcontrol
	PROPERTIES
	FOLDER Modules
)

# Installation
install(TARGETS Rcontrol
	RUNTIME
	DESTINATION ${SPACEXPANSE_INSTALL_PLUGIN_DIR}
)
