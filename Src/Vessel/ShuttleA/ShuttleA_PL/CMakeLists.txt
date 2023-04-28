# Copyright (c) Martin Schweiger
# Licensed under the MIT License

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SPACEXPANSE_BINARY_VESSEL_DIR})

set(MESH_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Meshes)
set(TEXTURE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Textures)
set(CONFIG_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Config)

add_library(ShuttleA_PL SHARED
	ShuttleA_pl.cpp
)

set_target_properties(ShuttleA_PL
	PROPERTIES
	FOLDER Vessels/ShuttleA
)

target_include_directories(ShuttleA_PL
	PUBLIC ${SPACEXPANSE_SOURCE_SDK_INCLUDE_DIR}
)

target_link_libraries(ShuttleA_PL
	${SPACEXPANSE_LIB}
	${SPACEXPANSE_SDK_LIB}
)

add_dependencies(ShuttleA_PL
	${SpaceXpanseTgt}
	SpaceXpansesdk
)

add_custom_command(
	TARGET ShuttleA_PL PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CONFIG_SOURCE_DIR}/ ${CMAKE_BINARY_DIR}/Config/Vessels
)

add_custom_command(
	TARGET ShuttleA_PL PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${MESH_SOURCE_DIR}/ ${CMAKE_BINARY_DIR}/Meshes/ShuttleA
)

add_custom_command(
	TARGET ShuttleA_PL PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${TEXTURE_SOURCE_DIR}/ ${SPACEXPANSE_BINARY_TEXTURES_DIR}/ShuttleA
)

# Installation
install(TARGETS ShuttleA_PL
	RUNTIME
	DESTINATION ${SPACEXPANSE_INSTALL_MODULE_DIR}
)
install(DIRECTORY ${CONFIG_SOURCE_DIR}/
	DESTINATION ${SPACEXPANSE_INSTALL_ROOT_DIR}/Config/Vessels
)
install(DIRECTORY ${MESH_SOURCE_DIR}/
	DESTINATION ${SPACEXPANSE_INSTALL_ROOT_DIR}/Meshes/ShuttleA
)
install(DIRECTORY ${TEXTURE_SOURCE_DIR}/
	DESTINATION ${SPACEXPANSE_INSTALL_TEXTURES_DIR}/ShuttleA
)
