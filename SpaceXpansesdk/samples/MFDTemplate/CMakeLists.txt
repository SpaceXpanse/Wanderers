# Copyright (c) Martin Schweiger
# Licensed under the MIT License

# The target directory for plugin DLLs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SPACEXPANSE_BINARY_PLUGIN_DIR})

add_library(MFDTemplate SHARED
	MFDTemplate.cpp
	MFDTemplate.rc
)

target_include_directories(MFDTemplate
	PUBLIC ${SPACEXPANSE_SOURCE_SDK_INCLUDE_DIR}
)

target_link_libraries(MFDTemplate
	${SPACEXPANSE_LIB}
	${SPACEXPANSE_SDK_LIB}
)

add_dependencies(MFDTemplate
	${SpaceXpanseTgt}
	SpaceXpansesdk
)

set_target_properties(LaunchpadParamTemplate
	PROPERTIES
	FOLDER SDKSamples
)
