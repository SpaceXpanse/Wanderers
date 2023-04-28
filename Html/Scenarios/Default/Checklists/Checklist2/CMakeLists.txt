set(name Checklist2)
set(subdir Scenarios/Default/Checklists)

file(GLOB ChmSources
	*.htm
	*.css
	*.jpg
	*.gif
	${name}.hhp
)

add_custom_command(OUTPUT ${SPACEXPANSE_BINARY_HTML_DIR}/${subdir}/${name}.chm
	COMMAND ${HHC_CMD} ${name}.hhp
	COMMAND ${CMAKE_COMMAND} -E rename ${name}.chm ${SPACEXPANSE_BINARY_HTML_DIR}/${subdir}/${name}.chm
	DEPENDS ${ChmSources}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	JOB_POOL htmlhelp
)

add_custom_target(${name}_chm
	DEPENDS ${SPACEXPANSE_BINARY_HTML_DIR}/${subdir}/${name}.chm
)
add_dependencies(${SpaceXpanseTgt}
	${name}_chm
)

install(FILES ${SPACEXPANSE_BINARY_HTML_DIR}/${subdir}/${name}.chm
	DESTINATION ${SPACEXPANSE_INSTALL_HTML_DIR}/${subdir}
)
