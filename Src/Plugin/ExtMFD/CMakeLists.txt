# Copyright (c) Martin Schweiger
# Licensed under the MIT License

add_library(ExtMFD SHARED
	ExtMFD.cpp
	ExtMFD.rc
	MFDWindow.cpp
)

target_include_directories(ExtMFD
	PUBLIC ${SPACEXPANSE_SOURCE_SDK_INCLUDE_DIR}
	PUBLIC ${MODULE_COMMON_DIR}
)

target_link_libraries(ExtMFD
	${SPACEXPANSE_LIB}
	${SPACEXPANSE_SDK_LIB}
)

add_dependencies(ExtMFD
	${SpaceXpanseTgt}
	SpaceXpansesdk
)

set_target_properties(ExtMFD
	PROPERTIES
	FOLDER Modules
)

# Installation
install(TARGETS ExtMFD
	RUNTIME
	DESTINATION ${SPACEXPANSE_INSTALL_PLUGIN_DIR}
)
