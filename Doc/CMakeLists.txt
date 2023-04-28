# SpaceXpanse.pdf ------------------------------------------------------------

odt_to_pdf_arglist("SpaceXpanse" arglist src out)
add_custom_command(
	OUTPUT ${out}
	COMMAND ${ODT_TO_PDF_COMPILER} ${arglist}
	DEPENDS ${src}
	JOB_POOL soffice
)
add_custom_target(SpaceXpanseDoc
	DEPENDS ${out}
)
add_dependencies(${SpaceXpanseTgt}
	SpaceXpanseDoc
)
set_target_properties(SpaceXpanseDoc
	PROPERTIES
	FOLDER Doc
)
install(FILES ${out}
	DESTINATION ${SPACEXPANSE_INSTALL_DOC_DIR}
)

# SpaceXpanseConfig.pdf ------------------------------------------------------

odt_to_pdf_arglist("SpaceXpanseConfig" arglist src out)
add_custom_command(
	OUTPUT ${out}
	COMMAND ${ODT_TO_PDF_COMPILER} ${arglist}
	DEPENDS ${src}
	JOB_POOL soffice
)
add_custom_target(SpaceXpanseConfigDoc
	DEPENDS ${out}
)
add_dependencies(${SpaceXpanseTgt}
	SpaceXpanseConfigDoc
)
set_target_properties(SpaceXpanseConfigDoc
	PROPERTIES
	FOLDER Doc
)
install(FILES ${out}
	DESTINATION ${SPACEXPANSE_INSTALL_DOC_DIR}
)

# PlanetTextures.pdf -----------------------------------------------------

odt_to_pdf_arglist("PlanetTextures" arglist src out)
add_custom_command(
	OUTPUT ${out}
	COMMAND ${ODT_TO_PDF_COMPILER} ${arglist}
	DEPENDS ${src}
	JOB_POOL soffice
)
add_custom_target(PlanetTexturesDoc
	DEPENDS ${out}
)
add_dependencies(${SpaceXpanseTgt}
	PlanetTexturesDoc
)
set_target_properties(PlanetTexturesDoc
	PROPERTIES
	FOLDER Doc
)
install(FILES ${out}
	DESTINATION ${SPACEXPANSE_INSTALL_DOC_DIR}
)

# Credit.pdf -------------------------------------------------------------

doc_to_pdf_arglist("Credit" arglist src out)
add_custom_command(
	OUTPUT ${out}
	COMMAND ${DOC_TO_PDF_COMPILER} ${arglist}
	DEPENDS ${src}
	JOB_POOL soffice
)
add_custom_target(SpaceXpanseCreditDoc
	DEPENDS ${out}
)
add_dependencies(SpaceXpanseDoc
	SpaceXpanseCreditDoc
)
install(FILES ${out}
	DESTINATION ${SPACEXPANSE_INSTALL_DOC_DIR}
)

# Technotes --------------------------------------------------------------

add_subdirectory(Technotes)
