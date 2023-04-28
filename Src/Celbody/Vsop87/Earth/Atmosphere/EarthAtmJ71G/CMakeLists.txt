# Copyright (c) Martin Schweiger
# Licensed under the MIT License

set(ATM_TARGET EarthAtmJ71G)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SPACEXPANSE_BINARY_CELBODY_DIR}/${CELBODY}/Atmosphere)

add_library(${ATM_TARGET} SHARED
	EarthAtmJ71G.cpp
)

add_dependencies(${ATM_TARGET}
	${SpaceXpanseTgt}
	SpaceXpansesdk
)

target_include_directories(${ATM_TARGET}
	PUBLIC ${CMAKE_SOURCE_DIR}/SpaceXpansesdk/include
)

target_link_libraries(${ATM_TARGET}
	${SPACEXPANSE_LIB}
	${SPACEXPANSE_SDK_LIB}
)

set_target_properties(${ATM_TARGET}
	PROPERTIES
	FOLDER Celbody/Atmospheres
)

# Installation
install(TARGETS ${ATM_TARGET}
	RUNTIME
	DESTINATION ${SPACEXPANSE_INSTALL_CELBODY_DIR}/${CELBODY}/Atmosphere
)
