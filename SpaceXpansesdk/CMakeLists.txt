# Copyright (c) Martin Schweiger
# Licensed under the MIT License

add_subdirectory(samples)

file(GLOB_RECURSE includes
	RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	LIST_DIRECTORIES false
	include/*
)
foreach(fname ${includes})
	list(APPEND input
		${CMAKE_CURRENT_SOURCE_DIR}/${fname}
	)
	list(APPEND output
		${SPACEXPANSE_BINARY_SDK_DIR}/${fname}
	)
	list(APPEND commands
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/${fname} ${SPACEXPANSE_BINARY_SDK_DIR}/${fname}
	)
endforeach()

add_custom_command(
	OUTPUT ${output}
	DEPENDS ${input}
	${commands}
)
add_custom_target(copy_sdk_include
	DEPENDS ${output}
)
add_dependencies(SpaceXpansesdk
	copy_sdk_include
)

if(SPACEXPANSE_MAKE_DOC)
	add_subdirectory(doc)
	if(Doxygen_FOUND)
		add_subdirectory(doxygen)
	endif()
endif()

install(DIRECTORY html include
	DESTINATION ${SPACEXPANSE_INSTALL_SDK_DIR}
)
