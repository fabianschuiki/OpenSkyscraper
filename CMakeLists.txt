cmake_minimum_required(VERSION 2.6)
project(OpenSkyscraper)

# Executable name.
set(EXECUTABLE_NAME OpenSkyscraper)

# Additional definitions.
if (MSVC)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DBUILD_DEBUG")
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /DBUILD_DEBUG")
	add_definitions(-Dsnprintf=_snprintf)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
	# Default to debug build.
	if (NOT CMAKE_BUILD_TYPE)
		set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
	endif()

	if (CMAKE_BUILD_TYPE MATCHES Debug)
		add_definitions("-DBUILD_DEBUG")
	else()
		add_definitions("-DBUILD_RELEASE")
	endif()
endif()

add_definitions("-Wno-switch")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DBUILD_DEBUG")

# Setup the version strings.
if (NOT VERSION)
	set(VERSION "dev" CACHE STRING "Version string (e.g. 1.2alpha1 or dev)" FORCE)
endif()

set(YEAR "" CACHE STRING "Year of compilation" FORCE)
if (NOT YEAR)
	if (UNIX)
		execute_process(COMMAND date +%Y OUTPUT_VARIABLE YEAR OUTPUT_STRIP_TRAILING_WHITESPACE)
	endif()
	mark_as_advanced(YEAR)
endif()

set(MACOSX_BUNDLE_BUNDLE_NAME ${EXECUTABLE_NAME})
set(MACOSX_BUNDLE_COPYRIGHT "Copyright © 2011-${YEAR} by OpenSkyscraper Project")
set(MACOSX_BUNDLE_BUNDLE_VERSION ${VERSION})

# Gather the source files.
file(GLOB_RECURSE sources RELATIVE ${PROJECT_SOURCE_DIR} source/*.cpp)

# TinyXML2
include_directories(thirdparty/tinyxml2)
list(APPEND sources thirdparty/tinyxml2/tinyxml2.cpp)


###  PLATFORM-SPECIFIC STUFF
###  =======================

# Mac OS X
if (APPLE)
	add_definitions("-DTARGET_APPLE")

	# Find the paths of all resources.
	file(GLOB_RECURSE resources RELATIVE ${PROJECT_SOURCE_DIR} data/*)

	# Make sure the resources are copied to the right bundle.
	foreach(r ${resources})
		get_filename_component(r_abs ${r} ABSOLUTE)
		get_filename_component(r_dir ${r_abs} PATH)
		file(RELATIVE_PATH r_rel ${PROJECT_SOURCE_DIR}/data ${r_dir})
		set_source_files_properties(${r} PROPERTIES MACOSX_PACKAGE_LOCATION Resources/${r_rel})
	endforeach()

	# Executable.
	add_executable(
		${EXECUTABLE_NAME} MACOSX_BUNDLE
		${sources}
		${resources}
	)

# other systems
else ()
	add_executable(
		${EXECUTABLE_NAME}
		${sources}
	)
endif ()


###  PACKAGES AND LIBRARIES
###  ======================

# Tell CMake to also search the thirdparty directory for modules.
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/thirdparty" ${CMAKE_MODULE_PATH})

# Include AStar-Search headers
include_directories(thirdparty/astar-search/)

# SFML
if (WIN32)
	set(SFMLDIR "" CACHE PATH "SFML directory")
endif ()
set(SFML_STATIC_LIBRARIES FALSE CACHE BOOL "Check if linking to SFML static libraries")
find_package(SFML REQUIRED audio graphics window system)
include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})

# Copy the SFML frameworks into the application bundle on Mac OS X.
if (APPLE)
	# install(DIRECTORY ${SFML_LIBRARIES} DESTINATION ${CMAKE_BINARY_DIR}/${EXECUTABLE_NAME}.app/Contents/Frameworks)
endif ()

# libRocket
find_package(Rocket REQUIRED core controls debugger)
string(REPLACE "optimized;${ROCKET_DEBUGGER_LIBRARY_RELEASE}" "" ROCKET_LIBRARIES "${ROCKET_LIBRARIES}")
include_directories(${ROCKET_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} ${ROCKET_LIBRARIES})

if (WIN32)
	set(FREETYPE_DIR "" CACHE PATH "freetype directory")
	set(ENV{FREETYPE_DIR} ${FREETYPE_DIR})
endif ()
find_package(Freetype REQUIRED)
target_link_libraries(${EXECUTABLE_NAME} ${FREETYPE_LIBRARIES})

# OpenGL (Needed by Rocket)
find_package(OpenGL REQUIRED)
target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_gl_LIBRARY})

# libmspack for decompressing KWAJ files (SIMTOWER.EX_)
# http://www.cabextract.org.uk/libmspack/
find_package(mspack)
if (MSPACK_FOUND)
	include_directories(${MSPACK_INCLUDE_DIRS})
	link_directories(${MSPACK_LIBRARY_DIRS})
	target_link_libraries(${EXECUTABLE_NAME} ${MSPACK_LIBRARIES})
	add_definitions(-DMSPACK)
endif (MSPACK_FOUND)
