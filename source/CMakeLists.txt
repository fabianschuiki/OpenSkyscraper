cmake_minimum_required(VERSION 2.6)
project(OpenSkyscraper)

include(BundleUtilities)

# Gather the frameworks
#set(FRAMEWORKS ../macosx/sfml/sfml-window.framework)
#set_source_files_properties(${FRAMEWORKS} PROPERTIES MACOSX_PACKAGE_LOCATION Frameworks)

set(sources
	main.cpp
)

add_executable(
	OpenSkyscraper MACOSX_BUNDLE
	${sources}
#	${FRAMEWORKS}
)

include_directories(../macosx/sfml/SFML.framework)

#link_directories(
#	../macosx/sfml/sfml-window.framework
#)

target_link_libraries(
	OpenSkyscraper
	"-F../macosx/sfml/"
	-framework\ sfml-window
)