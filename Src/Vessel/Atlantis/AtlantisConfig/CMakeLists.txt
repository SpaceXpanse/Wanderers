# Copyright (c) Martin Schweiger
# Licensed under the MIT License

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SPACEXPANSE_BINARY_STARTUP_DIR})

add_library(AtlantisConfig SHARED
	AtlantisConfig.cpp
	AConfigurator.rc
)

target_include_directories(AtlantisConfig
	PUBLIC ${SPACEXPANSE_SOURCE_SDK_INCLUDE_DIR}
)

target_link_libraries(AtlantisConfig
	${SPACEXPANSE_LIB}
	${SPACEXPANSE_SDK_LIB}
)

add_dependencies(AtlantisConfig
	${SpaceXpanseTgt}
	SpaceXpansesdk
)

set_target_properties(AtlantisConfig
	PROPERTIES
	FOLDER Vessels/Atlantis
)

# Installation
install(TARGETS AtlantisConfig
	RUNTIME
	DESTINATION ${SPACEXPANSE_INSTALL_MODULE_DIR}/Startup
)
