# Copyright (c) Martin Schweiger
# Licensed under the MIT License

# Core library to go to the SpaceXpanse main directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SPACEXPANSE_BINARY_ROOT_DIR})
set(BUILD_OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/out)

add_library(LuaInterpreter SHARED
	Interpreter.cpp
	lua_vessel_mtd.cpp
)

target_include_directories(LuaInterpreter
	PUBLIC ${SPACEXPANSE_SOURCE_SDK_INCLUDE_DIR}
)

target_link_libraries(LuaInterpreter
	${SPACEXPANSE_LIB}
	${SPACEXPANSE_SDK_LIB}
	${LUA_LIB}
)

add_dependencies(LuaInterpreter
	${SpaceXpanseTgt}
	SpaceXpansesdk
)

set_target_properties(LuaInterpreter
	PROPERTIES
	FOLDER Modules/Lua
)

# Copy library to its SpaceXpansesdk/lib location so that external project integrated into the build can find it
add_custom_command(TARGET LuaInterpreter
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E make_directory ${SPACEXPANSE_BINARY_SDK_DIR}/include
	COMMAND ${CMAKE_COMMAND} -E make_directory ${SPACEXPANSE_BINARY_SDK_DIR}/lib/Lua
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Interpreter.h ${SPACEXPANSE_BINARY_SDK_DIR}/include/
	COMMAND ${CMAKE_COMMAND} -E copy ${LUAINTERPRETER_LIB} ${SPACEXPANSE_BINARY_SDK_DIR}/lib/Lua/
)

if(SPACEXPANSE_MAKE_DOC)
	file(GLOB lua_in *.h *.cpp *.ld *.md *.lua)
	add_custom_command(
		OUTPUT ${BUILD_OUT_DIR}/index.html
		COMMAND ${CMAKE_COMMAND} -E copy ${lua_in} ${CMAKE_CURRENT_BINARY_DIR}
		COMMAND ${ldoc} .
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/interpreter.hhp ${BUILD_OUT_DIR}
		JOB_POOL htmlhelp
)
	add_custom_command(
		OUTPUT ${SPACEXPANSE_BINARY_SDK_DIR}/doc/spacexpanse_lua.chm
		DEPENDS ${BUILD_OUT_DIR}/index.html
		WORKING_DIRECTORY ${BUILD_OUT_DIR}
		COMMAND ${HHC_CMD} interpreter.hhp
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ./interpreter.chm ${SPACEXPANSE_BINARY_SDK_DIR}/doc/spacexpanse_lua.chm
		JOB_POOL htmlhelp
)
	add_custom_target(spacexpanse_lua
		DEPENDS ${SPACEXPANSE_BINARY_SDK_DIR}/doc/spacexpanse_lua.chm
	)
	add_dependencies(${SpaceXpanseTgt}
		spacexpanse_lua
	)

	install(FILES ${SPACEXPANSE_BINARY_SDK_DIR}/doc/spacexpanse_lua.chm
		DESTINATION ${SPACEXPANSE_INSTALL_SDK_DIR}/doc
	)
endif()

# Installation
install(TARGETS LuaInterpreter
	RUNTIME
	DESTINATION ${SPACEXPANSE_INSTALL_ROOT_DIR}
)
install(FILES $<TARGET_LINKER_FILE:LuaInterpreter>
	DESTINATION ${SPACEXPANSE_INSTALL_SDK_DIR}/lib/Lua
)