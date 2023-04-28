# Copyright (c) Martin Schweiger
# Licensed under the MIT License

add_library(LuaConsole SHARED
	LuaConsole.cpp
	LuaConsole.rc
	ConsoleInterpreter.cpp
	ConsoleCfg.cpp
)

target_include_directories(LuaConsole
	PUBLIC ${SPACEXPANSE_SOURCE_SDK_INCLUDE_DIR}
	PUBLIC ${MODULE_COMMON_DIR}
	PUBLIC ${SPACEXPANSE_SOURCE_ROOT_DIR}/Src/Module/LuaScript/LuaInterpreter
)

target_link_libraries(LuaConsole
	${SPACEXPANSE_LIB}
	${SPACEXPANSE_SDK_LIB}
	${LUA_LIB}
	${LUAINTERPRETER_LIB}
)

add_dependencies(LuaConsole
	${SpaceXpanseTgt}
	SpaceXpansesdk
	LuaInterpreter
)

set_target_properties(LuaConsole
	PROPERTIES
	FOLDER Modules
)

# Installation
install(TARGETS LuaConsole
	RUNTIME
	DESTINATION ${SPACEXPANSE_INSTALL_PLUGIN_DIR}
)
