# Copyright (c) Martin Schweiger
# Licensed under the MIT License

add_library(LuaMFD SHARED
	LuaMFD.cpp
	LuaMFD.rc
	MfdInterpreter.cpp
)

target_include_directories(LuaMFD
	PUBLIC ${SPACEXPANSE_SOURCE_SDK_INCLUDE_DIR}
	PUBLIC ${MODULE_COMMON_DIR}
	PUBLIC ${SPACEXPANSE_SOURCE_ROOT_DIR}/Src/Module/LuaScript/LuaInterpreter
)

target_link_libraries(LuaMFD
	${SPACEXPANSE_LIB}
	${SPACEXPANSE_SDK_LIB}
	${LUA_LIB}
	${LUAINTERPRETER_LIB}
)

add_dependencies(LuaMFD
	${SpaceXpanseTgt}
	SpaceXpansesdk
	LuaInterpreter
)

set_target_properties(LuaMFD
	PROPERTIES
	FOLDER Modules
)

# Installation
install(TARGETS LuaMFD
	RUNTIME
	DESTINATION ${SPACEXPANSE_INSTALL_PLUGIN_DIR}
)
