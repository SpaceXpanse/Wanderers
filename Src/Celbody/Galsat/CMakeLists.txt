# Copyright (c) Martin Schweiger
# Licensed under the MIT License

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Modules)

add_library(Galsat SHARED
	Galsat.cpp
	Lieske.cpp
)

set_target_properties(Galsat
	PROPERTIES
	FOLDER Celbody/Galsat
)

add_dependencies(Galsat
	${SpaceXpanseTgt}
	SpaceXpansesdk
)

target_include_directories(Galsat
	PUBLIC ${CMAKE_SOURCE_DIR}/SpaceXpansesdk/include
)

target_link_libraries(Galsat
	${SPACEXPANSE_LIB}
	${SPACEXPANSE_SDK_LIB}
)

add_custom_command(
	TARGET Galsat PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/Config/Jupiter/Data
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/ephem_e15.dat ${CMAKE_BINARY_DIR}/Config/Jupiter/Data/
)

set(GALSAT_LIB $<TARGET_LINKER_FILE:Galsat>)

add_subdirectory(Io)
add_subdirectory(Europa)
add_subdirectory(Ganymede)
add_subdirectory(Callisto)

# Installation
install(TARGETS
	Galsat
	RUNTIME
	DESTINATION ${SPACEXPANSE_INSTALL_MODULE_DIR}
)
install(FILES ephem_e15.dat
	DESTINATION ${SPACEXPANSE_INSTALL_ROOT_DIR}/Config/Jupiter/Data
)
