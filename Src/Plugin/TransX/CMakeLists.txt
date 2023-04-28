add_library(TransX SHARED
	BodyCache.cpp
	BodyProvider.cpp
    basefunction.cpp
    doublelink.cpp
    globals.cpp
    graph.cpp
    intercept.cpp
    mapfunction.cpp
    mfdfunction.cpp
    mfdvarhandler.cpp
    mfdvariable.cpp
    mfdvartypes.cpp
    orbitelements.cpp
    parser.cpp
    planfunction.cpp
    shiplist.cpp
    transx.cpp
    TransXFunction.cpp
    transxstate.cpp
    viewstate.cpp
    TransX.rc
)

target_include_directories(TransX
	PUBLIC ${SPACEXPANSE_SOURCE_SDK_INCLUDE_DIR}
	PUBLIC ${MODULE_COMMON_DIR}
)

target_link_libraries(TransX
	${SPACEXPANSE_LIB}
	${SPACEXPANSE_SDK_LIB}
)

add_dependencies(TransX
	${SpaceXpanseTgt}
	SpaceXpansesdk
)

set_target_properties(TransX
	PROPERTIES
	FOLDER Modules
)

add_custom_command(
    TARGET TransX PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Doc ${SPACEXPANSE_BINARY_DOC_DIR}/TransX
)

# Installation
install(TARGETS TransX
	RUNTIME
	DESTINATION ${SPACEXPANSE_INSTALL_PLUGIN_DIR}
)
install(DIRECTORY ${SPACEXPANSE_BINARY_DOC_DIR}/TransX
    DESTINATION ${SPACEXPANSE_INSTALL_DOC_DIR}
)
