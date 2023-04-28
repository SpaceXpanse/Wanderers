set(name aap)
set(subdir Script/Stockvessels/DG)

file(GLOB ChmSources
	*.htm
	*.css
	*.jpg
	*.gif
	*.hhc
	*.hhk
	${name}.hhp
)

# copy everything to the build directory and compile there

add_custom_command(OUTPUT ${SPACEXPANSE_BINARY_HTML_DIR}/${subdir}/${name}.chm
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/ ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND ${HHC_CMD} ${name}.hhp
	COMMAND ${CMAKE_COMMAND} -E copy ${name}.chm ${SPACEXPANSE_BINARY_HTML_DIR}/${subdir}/
	DEPENDS ${ChmSources}
	JOB_POOL htmlhelp
)
add_custom_target(${name}_chm
	DEPENDS ${SPACEXPANSE_BINARY_HTML_DIR}/${subdir}/${name}.chm
)
add_dependencies(DeltaGlider
	${name}_chm
)

install(FILES ${SPACEXPANSE_BINARY_HTML_DIR}/${subdir}/${name}.chm
	DESTINATION ${SPACEXPANSE_INSTALL_HTML_DIR}/${subdir}
)
