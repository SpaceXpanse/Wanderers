# Copyright (c) Martin Schweiger
# Licensed under the MIT License

set(CELBODY "Moon")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Modules/Celbody)

add_library(${CELBODY} SHARED
	${CELBODY}.cpp
	ELP82.cpp
)

add_dependencies(${CELBODY}
	${SpaceXpanseTgt}
	SpaceXpansesdk
)

target_include_directories(${CELBODY}
	PUBLIC ${CMAKE_SOURCE_DIR}/SpaceXpansesdk/include
)

target_link_libraries(${CELBODY}
	${SPACEXPANSE_LIB}
	${SPACEXPANSE_SDK_LIB}
)

add_custom_command(
	TARGET ${CELBODY} PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Config/ ${CMAKE_BINARY_DIR}/Config	
)

set_target_properties(${CELBODY}
	PROPERTIES
	FOLDER Celbody
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
