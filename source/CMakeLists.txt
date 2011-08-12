cmake_minimum_required(VERSION 2.6)
project(OpenSkyscraper)

set(sources
	main.cpp
)

include_directories(../macosx/sfml/SFML.framework)
set(frameworks
	../macosx/sfml/sfml-system.framework
	../macosx/sfml/sfml-window.framework
	../macosx/sfml/sfml-graphics.framework
)
set(FRAMEWORKS_DIR ${CMAKE_BINARY_DIR}/OpenSkyscraper.app/Contents/Frameworks)
install(DIRECTORY ${frameworks} DESTINATION ${FRAMEWORKS_DIR})


# Find all data files that need to be copied.
unset(dataFiles)
macro(copyFiles srcDir dstDir)
	message("Copying " ${srcDir})
	file(GLOB tmpFiles RELATIVE ${srcDir} ${srcDir}/*)
	foreach(tmpFile ${tmpFiles})
		set(srcTmpPath ${srcDir}/${tmpFile})
		set(dstTmpPath ${dstDir}/${tmpFile})
		if(IS_DIRECTORY ${srcTmpPath})
			copyFiles(${srcTmpPath} ${dstTmpPath})
		else()
			message("Adding data file " ${dstTmpPath})
			list(APPEND dataFiles ${srcTmpPath})
			set_source_files_properties(
				${srcTmpPath}
				PROPERTIES MACOSX_PACKAGE_LOCATION Resources/${dstDir}
			)
        endif(IS_DIRECTORY ${srcTmpPath})
	endforeach(tmpFile)
endmacro(copyFiles)
copyFiles(${CMAKE_SOURCE_DIR}/../data "")
message("Data Files: " ${dataFiles})

#set_source_files_properties(
#	../data/fonts/Jura-Regular.ttf
#	PROPERTIES
#	MACOSX_PACKAGE_LOCATION Resources/fonts
#)


add_executable(
	OpenSkyscraper MACOSX_BUNDLE
	${sources}
	#../data/fonts/Jura-Regular.ttf
	${dataFiles}
)

target_link_libraries(
	OpenSkyscraper
	"-F../macosx/sfml/"
	"-framework sfml-system"
	"-framework sfml-window"
	"-framework sfml-graphics"
)
