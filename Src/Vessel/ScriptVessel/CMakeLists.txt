# Copyright (c) Martin Schweiger
# Licensed under the MIT License

add_library(ScriptVessel SHARED
	ScriptVessel.cpp
)

set_target_properties(ScriptVessel
	PROPERTIES
	FOLDER Vessels
)

target_include_directories(ScriptVessel
	PUBLIC ${SPACEXPANSE_SOURCE_SDK_INCLUDE_DIR}
)

target_link_libraries(ScriptVessel
	${SPACEXPANSE_LIB}
	${SPACEXPANSE_SDK_LIB}
	${LUA_LIB}
)

add_dependencies(ScriptVessel
	${SpaceXpanseTgt}
	SpaceXpansesdk
)

# Installation
install(TARGETS ScriptVessel
	RUNTIME
	DESTINATION ${SPACEXPANSE_INSTALL_MODULE_DIR}
)
