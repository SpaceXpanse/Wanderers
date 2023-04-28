# Copyright (c) Martin Schweiger
# Licensed under the MIT License

add_library(Meshdebug SHARED
	Meshdebug.cpp
	Meshdebug.rc
)

target_include_directories(Meshdebug
	PUBLIC ${SPACEXPANSE_SOURCE_SDK_INCLUDE_DIR}
	PUBLIC ${MODULE_COMMON_DIR}
)

target_link_libraries(Meshdebug
	${SPACEXPANSE_LIB}
	${SPACEXPANSE_SDK_LIB}
)

add_dependencies(Meshdebug
	${SpaceXpanseTgt}
	SpaceXpansesdk
)

set_target_properties(Meshdebug
	PROPERTIES
	FOLDER Modules
)

# Installation
install(TARGETS Meshdebug
	RUNTIME
	DESTINATION ${SPACEXPANSE_INSTALL_PLUGIN_DIR}
)
