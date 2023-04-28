# Copyright (c) Martin Schweiger
# Licensed under the MIT License

add_library(ScriptMFD SHARED
	ScriptMFD.cpp
	ScriptMFD.rc
)

target_include_directories(ScriptMFD
	PUBLIC ${SPACEXPANSE_SOURCE_SDK_INCLUDE_DIR}
	PUBLIC ${MODULE_COMMON_DIR}
	PUBLIC ${SPACEXPANSE_SOURCE_ROOT_DIR}/Src/Module/LuaScript/LuaInterpreter
)

target_link_libraries(ScriptMFD
	${SPACEXPANSE_LIB}
	${SPACEXPANSE_SDK_LIB}
	${LUA_LIB}
	${LUAINTERPRETER_LIB}
)

add_dependencies(ScriptMFD
	${SpaceXpanseTgt}
	SpaceXpansesdk	
	LuaInterpreter
)

set_target_properties(ScriptMFD
	PROPERTIES
	FOLDER Modules
)

# Installation
install(TARGETS ScriptMFD
	RUNTIME
	DESTINATION ${SPACEXPANSE_INSTALL_PLUGIN_DIR}
)
