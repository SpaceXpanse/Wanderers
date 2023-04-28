# API_Guide.pdf ----------------------------------------------------------

doc_to_pdf_arglist("API_Guide" arglist src out)
add_custom_command(
	OUTPUT ${out}
	COMMAND ${DOC_TO_PDF_COMPILER} ${arglist}
	DEPENDS ${src}
	JOB_POOL soffice
)
add_custom_target(APIGuideDoc
	DEPENDS ${out}
)
add_dependencies(${SpaceXpanseTgt}
	APIGuideDoc
)
install(FILES ${out}
	DESTINATION ${SPACEXPANSE_INSTALL_SDK_DIR}/doc
)

# 3DModel.pdf ------------------------------------------------------------

doc_to_pdf_arglist("3DModel" arglist src out)
add_custom_command(
	OUTPUT ${out}
	COMMAND ${DOC_TO_PDF_COMPILER} ${arglist}
	DEPENDS ${src}
	JOB_POOL soffice
)
add_custom_target(3DModelDoc
	DEPENDS ${out}
)
add_dependencies(${SpaceXpanseTgt}
	3DModelDoc
)
install(FILES ${out}
	DESTINATION ${SPACEXPANSE_INSTALL_SDK_DIR}/doc
)
