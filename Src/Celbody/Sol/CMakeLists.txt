# Copyright (c) Martin Schweiger
# Licensed under the MIT License

srcdir_to_tgtlist("${CMAKE_CURRENT_SOURCE_DIR}/Config" "${SPACEXPANSE_BINARY_CONFIG_DIR}" srclist tgtlist)
add_custom_command(
	OUTPUT ${tgtlist}
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Config/ ${SPACEXPANSE_BINARY_CONFIG_DIR}
	DEPENDS ${srclist}
)
add_custom_target(Sol
	DEPENDS ${tgtlist}
)
add_dependencies(${SpaceXpanseTgt}
	Sol
)

set_target_properties(Sol
	PROPERTIES
	FOLDER Celbody
)

# Installation
install(DIRECTORY Config
	DESTINATION ${SPACEXPANSE_INSTALL_ROOT_DIR}
)