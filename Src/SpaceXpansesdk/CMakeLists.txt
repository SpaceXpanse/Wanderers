# Copyright (c) Martin Schweiger
# Licensed under the MIT License

# Static library containing the DLL entry point for SpaceXpanse plugins.
# To be included by all SpaceXpanse plugin DLLs.

add_library(SpaceXpansesdk STATIC
	SpaceXpansesdk.cpp
)

set_target_properties(SpaceXpansesdk
	PROPERTIES
	FOLDER Core
)

# Copy library to its SpaceXpansesdk/lib location so that external project integrated into the build can find it
add_custom_command(TARGET SpaceXpansesdk
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E make_directory ${SPACEXPANSE_BINARY_SDK_DIR}/lib
	COMMAND ${CMAKE_COMMAND} -E copy ${SPACEXPANSE_SDK_LIB} ${SPACEXPANSE_BINARY_SDK_DIR}/lib/ 
)

# Installation: SpaceXpanse addon entry interface library to SDK folder to be included by addons
install(TARGETS SpaceXpansesdk
	ARCHIVE
	DESTINATION ${SPACEXPANSE_INSTALL_SDK_DIR}/lib
)
