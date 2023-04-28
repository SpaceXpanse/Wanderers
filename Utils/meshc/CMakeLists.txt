# Copyright (c) Martin Schweiger
# Licensed under the MIT License

add_executable(meshc
	meshc.cpp
	Mesh.cpp
)

target_include_directories(meshc
	PUBLIC ${SPACEXPANSE_SOURCE_DIR}
)

set_target_properties(meshc
	PROPERTIES
	FOLDER Tools
)

install(TARGETS meshc
	DESTINATION ${SPACEXPANSE_INSTALL_SDK_DIR}/Utils
)
