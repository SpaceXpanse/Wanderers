# Copyright (c) Martin Schweiger
# Licensed under the MIT License

# The target directory for plugin DLLs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SPACEXPANSE_BINARY_PLUGIN_DIR})

add_library(AscentMFD SHARED
	AscentMFD.cpp
	AscentMFD.rc
)

target_include_directories(AscentMFD
	PUBLIC ${SPACEXPANSE_SOURCE_SDK_INCLUDE_DIR}
)

target_link_libraries(AscentMFD
	${SPACEXPANSE_LIB}
	${SPACEXPANSE_SDK_LIB}
)

add_dependencies(AscentMFD
	${SpaceXpanseTgt}
	SpaceXpansesdk
)

set_target_properties(AscentMFD
	PROPERTIES
	FOLDER SDKSamples
)
