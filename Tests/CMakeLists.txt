Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.0-preview3
)

FetchContent_MakeAvailable(Catch2)

# Utility function
function(add_test_file test_name)
	add_executable(${test_name} "${test_name}.cpp")

	set_target_properties( ${test_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${SPACEXPANSE_BINARY_ROOT_DIR}" )

	target_include_directories(${test_name}
		PRIVATE ${SPACEXPANSE_SOURCE_SDK_INCLUDE_DIR}
		PRIVATE ${MODULE_COMMON_DIR}
		PRIVATE ${SPACEXPANSE_SOURCE_ROOT_DIR}/Src/Module/LuaScript/LuaInterpreter
	)

	target_link_libraries(${test_name}
		${SPACEXPANSE_LIB}
		${SPACEXPANSE_SDK_LIB}
		${LUA_LIB}
		${LUAINTERPRETER_LIB}
		Catch2::Catch2WithMain
	)

	add_dependencies(${test_name}
		SpaceXpansesdk
		LuaInterpreter
	)

	if (BUILD_SPACEXPANSE_DX7)
		add_dependencies(${test_name} SpaceXpanse)
	else()
		add_dependencies(${test_name} SpaceXpanse_server)
	endif()
	
	add_test( 
		NAME ${test_name} 
		COMMAND $<TARGET_FILE:${test_name}>
		WORKING_DIRECTORY ${SPACEXPANSE_BINARY_ROOT_DIR}
	)
	set_tests_properties(${test_name} PROPERTIES TIMEOUT 30)
endfunction()

# Register unit tests
add_test_file(Lua.Interpreter)

# Sanity check for scenario tests
add_test(
	NAME "Scenario.SanityCheck"
	COMMAND $<TARGET_FILE:SpaceXpanse_server> "--scenariox=${CMAKE_SOURCE_DIR}/Scenarios/Today.scn" "--maxframes=60"
	WORKING_DIRECTORY ${SPACEXPANSE_BINARY_ROOT_DIR}
)
set_tests_properties(Scenario.SanityCheck PROPERTIES TIMEOUT 60)

# Register scenario tests
file(GLOB TestScenarios "${CMAKE_SOURCE_DIR}/Scenarios/Tests/*.scn")
foreach(Scenario ${TestScenarios})
	get_filename_component(test_name ${Scenario} NAME_WE)
	add_test(
		NAME "Scenario.${test_name}"
		COMMAND $<TARGET_FILE:SpaceXpanse_server> "--scenariox=${Scenario}"
		WORKING_DIRECTORY ${SPACEXPANSE_BINARY_ROOT_DIR}
	)
	set_tests_properties(Scenario.${test_name} PROPERTIES TIMEOUT 60)
endforeach()
