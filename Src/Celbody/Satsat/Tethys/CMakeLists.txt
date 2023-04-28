# Copyright (c) Martin Schweiger
# Licensed under the MIT License

set(CELBODY "Tethys")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Modules/Celbody)

add_library(${CELBODY} SHARED
	${CELBODY}.cpp
)

add_dependencies(${CELBODY}
	Satsat
	${SpaceXpanseTgt}
	SpaceXpansesdk
)

target_include_directories(${CELBODY}
	PUBLIC ${CMAKE_SOURCE_DIR}/SpaceXpansesdk/include
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(${CELBODY}
	${SATSAT_LIB}
	${SPACEXPANSE_LIB}
	${SPACEXPANSE_SDK_LIB}
)

add_custom_command(
	TARGET ${CELBODY} PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Config/ ${CMAKE_BINARY_DIR}/Config	
)

set_target_properties(${CELBODY}
	PROPERTIES
	FOLDER Celbody/Satsat
)

#Installation
install(TARGETS
	${CELBODY}
	RUNTIME
	DESTINATION ${SPACEXPANSE_INSTALL_CELBODY_DIR}
)
install(DIRECTORY Config
	DESTINATION ${SPACEXPANSE_INSTALL_ROOT_DIR}
)
