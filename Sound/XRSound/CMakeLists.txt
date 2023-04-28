if (NOT EXISTS ${IRRKLANG_DIR})
	string(COMPARE EQUAL ${IRRKLANG_DIR} "irrKlang_DOWNLOAD" IRRKLANG_DOWNLOAD)
	if (IRRKLANG_DOWNLOAD)
		# Try downloading the irrKlang library on the fly
		set(IRRKLANG_DOWNLOAD_DIR ${SPACEXPANSE_BINARY_ROOT_DIR}/Extern/irrKlang/${ARCH}/)
		if (BUILD64)
			set(IRRKLANG_URL "https://www.ambiera.at/downloads/irrKlang-64bit-1.6.0.zip")
			set(IRRKLANG_DIR ${IRRKLANG_DOWNLOAD_DIR}/irrKlang-64bit-1.6.0)
		else()
			set(IRRKLANG_URL "https://www.ambiera.at/downloads/irrKlang-32bit-1.6.0.zip")
			set(IRRKLANG_DIR ${IRRKLANG_DOWNLOAD_DIR}/irrKlang-1.6.0)
		endif()
		if (EXISTS ${IRRKLANG_DIR})
			message(STATUS "Found a previously downloaded irrKlang package.")
		else()
			message(STATUS "Downloading irrKlang ...")
			file(MAKE_DIRECTORY ${IRRKLANG_DOWNLOAD_DIR})
			file(DOWNLOAD ${IRRKLANG_URL} ${IRRKLANG_DOWNLOAD_DIR}/irrKlang.zip)
			file(ARCHIVE_EXTRACT INPUT ${IRRKLANG_DOWNLOAD_DIR}/irrKlang.zip
				DESTINATION ${IRRKLANG_DOWNLOAD_DIR})
		endif()
	endif()
endif()

# The irrKlang library location
if (BUILD64)
	set(IRRKLANG_LIB_DIR "${IRRKLANG_DIR}/lib/Winx64-visualStudio")
	set(IRRKLANG_BIN_DIR "${IRRKLANG_DIR}/bin/Winx64-visualStudio")
else()
	set(IRRKLANG_LIB_DIR "${IRRKLANG_DIR}/lib/Win32-visualStudio")
	set(IRRKLANG_BIN_DIR "${IRRKLANG_DIR}/bin/Win32-visualStudio")
endif()
set(IRRKLANG_LIB "${IRRKLANG_LIB_DIR}/irrKlang.lib")

if(EXISTS ${IRRKLANG_LIB})

	set(ASSET_DIR ${CMAKE_CURRENT_SOURCE_DIR}/XRSound/assets)
	set(SOLUTION_DIR ${CMAKE_CURRENT_SOURCE_DIR}/XRSound/src)
	set(IRRKLANG_DLL_DIR ${IRRKLANG_DIR})
	# Deploy assets
	file(GLOB config_files ${ASSET_DIR}/XRSound/*.cfg)

	add_custom_target(XRSound_assets ALL
		COMMAND ${CMAKE_COMMAND} -E make_directory ${SPACEXPANSE_BINARY_ROOT_DIR}/Doc
		COMMAND ${CMAKE_COMMAND} -E make_directory ${SPACEXPANSE_BINARY_ROOT_DIR}/XRSound
		COMMAND ${CMAKE_COMMAND} -E make_directory ${SPACEXPANSE_BINARY_SDK_DIR}/XRSound/
		COMMAND ${CMAKE_COMMAND} -E copy ${config_files} ${SPACEXPANSE_BINARY_ROOT_DIR}/XRSound/
		COMMAND ${CMAKE_COMMAND} -E copy ${ASSET_DIR}/XRSound/ReadMe.txt ${SPACEXPANSE_BINARY_ROOT_DIR}/XRSound/
		COMMAND ${CMAKE_COMMAND} -E copy "${ASSET_DIR}/Doc/XRSound User Manual.pdf" ${SPACEXPANSE_BINARY_ROOT_DIR}/Doc/
		COMMAND ${CMAKE_COMMAND} -E copy ${SOLUTION_DIR}/XRSoundLib/XRSound.h ${SPACEXPANSE_BINARY_SDK_DIR}/XRSound/
		COMMAND ${CMAKE_COMMAND} -E copy ${IRRKLANG_BIN_DIR}/ikpMP3.dll ${SPACEXPANSE_BINARY_ROOT_DIR}
		COMMAND ${CMAKE_COMMAND} -E copy ${IRRKLANG_BIN_DIR}/ikpFlac.dll ${SPACEXPANSE_BINARY_ROOT_DIR}
		COMMAND ${CMAKE_COMMAND} -E copy ${IRRKLANG_BIN_DIR}/irrKlang.dll ${SPACEXPANSE_BINARY_ROOT_DIR}
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${ASSET_DIR}/XRSound/Default ${SPACEXPANSE_BINARY_ROOT_DIR}/XRSound/Default
	)

	install(DIRECTORY ${SPACEXPANSE_BINARY_ROOT_DIR}/XRSound
		DESTINATION ${SPACEXPANSE_INSTALL_ROOT_DIR}
	)
	install(DIRECTORY ${SPACEXPANSE_BINARY_SDK_DIR}/XRSound
		DESTINATION ${SPACEXPANSE_INSTALL_SDK_DIR}
	)
	install(DIRECTORY ${SPACEXPANSE_BINARY_ROOT_DIR}/XRSound/Default
		DESTINATION ${SPACEXPANSE_BINARY_ROOT_DIR}/XRSound
	)
	install(FILES "${SPACEXPANSE_BINARY_ROOT_DIR}/Doc/XRSound User Manual.pdf"
		DESTINATION ${SPACEXPANSE_INSTALL_DOC_DIR}/
	)
	install(FILES ${SPACEXPANSE_BINARY_ROOT_DIR}/ikpMP3.dll ${SPACEXPANSE_BINARY_ROOT_DIR}/ikpFlac.dll ${SPACEXPANSE_BINARY_ROOT_DIR}/irrKlang.dll
		DESTINATION ${SPACEXPANSE_INSTALL_ROOT_DIR}/
	)

	add_subdirectory(XRSound)

else()

	message(WARNING "irrKlang libraries not found: not building XRSound module.")

endif()
