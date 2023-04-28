add_subdirectory(Scenarios)

add_custom_command(
	OUTPUT credit.chm
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/credit.chm.
)

install(FILES credit.chm
	DESTINATION ${SPACEXPANSE_INSTALL_HTML_DIR}
)

if(SPACEXPANSE_MAKE_DOC)
	add_subdirectory(Main)
endif()
