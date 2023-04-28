# Copyright (c) Martin Schweiger
# Licensed under the MIT License

add_library(ScnEditor SHARED
	ScnEditor.cpp
	Editor.cpp
	Convert.cpp
	ScnEditor.rc
)

target_include_directories(ScnEditor
	PUBLIC ${SPACEXPANSE_SOURCE_SDK_INCLUDE_DIR}
)

target_link_libraries(ScnEditor
	${SPACEXPANSE_LIB}
	${SPACEXPANSE_SDK_LIB}
	${SPACEXPANSE_DLGCTRL_LIB}
	comctl32.lib
)

set_target_properties(ScnEditor
	PROPERTIES
	FOLDER Modules
)

# Deploy the API header file
add_custom_command(
	OUTPUT ${SPACEXPANSE_BINARY_SDK_DIR}/include/ScnEditorAPI.h
	COMMAND ${CMAKE_COMMAND} -E make_directory ${SPACEXPANSE_BINARY_SDK_DIR}/include
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/ScnEditorAPI.h ${SPACEXPANSE_BINARY_SDK_DIR}/include
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/ScnEditorAPI.h
)
add_custom_target(deploy_api
	DEPENDS ${SPACEXPANSE_BINARY_SDK_DIR}/include/ScnEditorAPI.h
)

add_dependencies(ScnEditor
	${SpaceXpanseTgt}
	SpaceXpansesdk
	DlgCtrl
	deploy_api
)

# Installation
install(TARGETS ScnEditor
	RUNTIME
	DESTINATION ${SPACEXPANSE_INSTALL_PLUGIN_DIR}
)
install(FILES ScnEditorAPI.h
	DESTINATION ${SPACEXPANSE_INSTALL_SDK_DIR}/include
)

if(SPACEXPANSE_MAKE_DOC)
	add_subdirectory(Doc)
	add_subdirectory(Help)
endif()