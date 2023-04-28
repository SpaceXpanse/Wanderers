# Copyright (c) Martin Schweiger
# Licensed under the MIT License

add_library(DlgCtrl STATIC
	DlgCtrl.cpp
	DlgCtrlPropertyList.cpp
	DlgCtrlSwitch.cpp
)

target_include_directories(DlgCtrl
	PUBLIC ${CMAKE_SOURCE_DIR}/SpaceXpansesdk/include
)

set_target_properties(DlgCtrl
	PROPERTIES
	FOLDER Core
)

# Copy library to its SpaceXpansesdk/lib location so that external project integrated into the build can find it
add_custom_command(TARGET DlgCtrl
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E make_directory ${SPACEXPANSE_BINARY_SDK_DIR}/lib
	COMMAND ${CMAKE_COMMAND} -E copy ${SPACEXPANSE_DLGCTRL_LIB} ${SPACEXPANSE_BINARY_SDK_DIR}/lib/ 
)

# Installation to SDK lib dir for inclusion by addons
install(TARGETS DlgCtrl
	ARCHIVE
	DESTINATION ${SPACEXPANSE_INSTALL_SDK_DIR}/lib
)
