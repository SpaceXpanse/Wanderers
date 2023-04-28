# Copyright (c) Martin Schweiger
# Licensed under the MIT License

add_executable(SpaceXpanse_ng
	SpaceXpanse_ng.cpp
	SpaceXpanse_ng.rc
)

set_target_properties(SpaceXpanse_ng
	PROPERTIES
	LINK_FLAGS "/SUBSYSTEM:WINDOWS"
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
	FOLDER Core
)

#Installation
install(TARGETS
	SpaceXpanse_ng
	RUNTIME
	DESTINATION ${SPACEXPANSE_INSTALL_ROOT_DIR}
	)
