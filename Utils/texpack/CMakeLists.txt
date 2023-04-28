# Copyright (c) Martin Schweiger
# Licensed under the MIT License

add_executable(texpack
	texpack.cpp
)

target_include_directories(texpack
	PUBLIC ${ZLIB_INCLUDE_DIR}
)

target_link_libraries(texpack
	Shlwapi.lib
	${ZLIB_LIB}
)

set_target_properties(texpack
	PROPERTIES
	FOLDER Tools
)

# Installation
install(TARGETS
	texpack
	RUNTIME
	DESTINATION ${SPACEXPANSE_INSTALL_UTILS_DIR}
)