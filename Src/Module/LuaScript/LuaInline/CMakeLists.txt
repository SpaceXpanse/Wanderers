# Copyright (c) Martin Schweiger
# Licensed under the MIT License

# Core library to go to the SpaceXpanse main directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SPACEXPANSE_BINARY_ROOT_DIR})

add_library(LuaInline SHARED
	LuaInline.cpp
)

target_include_directories(LuaInline
	PUBLIC ${SPACEXPANSE_SOURCE_SDK_INCLUDE_DIR}
	PUBLIC ${SPACEXPANSE_SOURCE_ROOT_DIR}/Src/Module/LuaScript/LuaInterpreter
)

target_link_libraries(LuaInline
	${SPACEXPANSE_LIB}
	${SPACEXPANSE_SDK_LIB}
	LuaInterpreter
	${LUA_LIB}
)

add_dependencies(LuaInline
	${SpaceXpanseTgt}
	SpaceXpansesdk
	LuaInterpreter
)

set_target_properties(LuaInline
	PROPERTIES
	FOLDER Modules/Lua
)

# Installation
install(TARGETS LuaInline
	RUNTIME
	DESTINATION ${SPACEXPANSE_INSTALL_ROOT_DIR}
)
