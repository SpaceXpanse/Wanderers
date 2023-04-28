# The XRSoundLib library target
add_library(XRSound_lib STATIC
	XRSound.cpp
	XRSoundImpl.cpp
)

set_target_properties(XRSound_lib
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ${SPACEXPANSE_BINARY_SDK_DIR}/$<1:XRSound>
)
if(FORCE_DEBUG)
	set_target_properties(XRSound_lib
		PROPERTIES
		OUTPUT_NAME XRSoundD
	)
else()
	set_target_properties(XRSound_lib
		PROPERTIES
		OUTPUT_NAME XRSound
	)
endif()

target_include_directories(XRSound_lib PUBLIC
	${SPACEXPANSE_SOURCE_SDK_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../XRSoundDLL
)

install(TARGETS XRSound_lib
	ARCHIVE
	DESTINATION ${SPACEXPANSE_INSTALL_SDK_DIR}/XRSound
)
