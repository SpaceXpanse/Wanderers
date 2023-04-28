# Copyright (c) Martin Schweiger
# Licensed under the MIT License

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SPACEXPANSE_BINARY_MODULE_DIR}/Server)

add_definitions(-DGDICLIENT_EXPORTS)

add_library(GDIClient SHARED
	GDIClient.cpp
)

target_include_directories(GDIClient
	PUBLIC ${CMAKE_SOURCE_DIR}/SpaceXpansesdk/include
)

add_dependencies(GDIClient
	${SpaceXpanseTgt}
	SpaceXpansesdk
)

target_link_libraries(GDIClient
	${SPACEXPANSE_LIB}
	${SPACEXPANSE_SDK_LIB}
)

# Installation
install(TARGETS
	GDIClient
	RUNTIME
	DESTINATION ${SPACEXPANSE_INSTALL_MODULE_DIR}/Server
)
