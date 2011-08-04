cmake_minimum_required(VERSION 2.6)
project(OpenSkyscraper)

set(sources
	main.cpp
)

include_directories(../macosx/sfml/SFML.framework)
set(frameworks
	../macosx/sfml/sfml-system.framework
	../macosx/sfml/sfml-window.framework
)
set(FRAMEWORKS_DIR ${CMAKE_BINARY_DIR}/OpenSkyscraper.app/Contents/Frameworks)
install(DIRECTORY ${frameworks} DESTINATION ${FRAMEWORKS_DIR})

add_executable(
	OpenSkyscraper MACOSX_BUNDLE
	${sources}
)

target_link_libraries(
	OpenSkyscraper
	"-F../macosx/sfml/"
	-framework\ sfml-window
)
