# Copyright (c) Martin Schweiger
# Licensed under the MIT License

add_library(FlightData SHARED
	FlightData.cpp
	FlightData.rc
	FDGraph.cpp
	../Common/Dialog/Graph.cpp
)

target_include_directories(FlightData
	PUBLIC ${SPACEXPANSE_SOURCE_SDK_INCLUDE_DIR}
	PUBLIC ${MODULE_COMMON_DIR}
)

target_link_libraries(FlightData
	${SPACEXPANSE_LIB}
	${SPACEXPANSE_SDK_LIB}
)

add_dependencies(FlightData
	${SpaceXpanseTgt}
	SpaceXpansesdk
)

set_target_properties(FlightData
	PROPERTIES
	FOLDER Modules
)

# Installation
install(TARGETS FlightData
	RUNTIME
	DESTINATION ${SPACEXPANSE_INSTALL_PLUGIN_DIR}
)
