# Copyright (c) Martin Schweiger
# Licensed under the MIT License

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SPACEXPANSE_BINARY_STARTUP_DIR})

add_library(DGConfigurator SHARED
	DGConfigurator.cpp
	DGConfigurator.rc
)

target_include_directories(DGConfigurator
	PUBLIC ${SPACEXPANSE_SOURCE_SDK_INCLUDE_DIR}
)

target_link_libraries(DGConfigurator
	${SPACEXPANSE_LIB}
	${SPACEXPANSE_SDK_LIB}
)

add_dependencies(DGConfigurator
	${SpaceXpanseTgt}
	SpaceXpansesdk
)

set_target_properties(DGConfigurator
	PROPERTIES
	FOLDER Vessels/DeltaGlider
)

# Installation
install(TARGETS DGConfigurator
	RUNTIME
	DESTINATION ${SPACEXPANSE_INSTALL_MODULE_DIR}/Startup
)
