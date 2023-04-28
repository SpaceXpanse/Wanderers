# Copyright (c) Martin Schweiger
# Licensed under the MIT License

# The target directory for plugin DLLs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SPACEXPANSE_BINARY_PLUGIN_DIR})

add_library(DialogTemplate SHARED
	DialogTemplate.cpp
	DialogTemplate.rc
)

target_include_directories(DialogTemplate
	PUBLIC ${SPACEXPANSE_SOURCE_SDK_INCLUDE_DIR}
)

target_link_libraries(DialogTemplate
	${SPACEXPANSE_LIB}
	${SPACEXPANSE_SDK_LIB}
)

add_dependencies(DialogTemplate
	${SpaceXpanseTgt}
	SpaceXpansesdk
)

set_target_properties(DialogTemplate
	PROPERTIES
	FOLDER SDKSamples
)
