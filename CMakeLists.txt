cmake_minimum_required(VERSION 2.6)
project(OpenSkyscraper)

# Default to debug build.
if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# Executable name.
set(EXECUTABLE_NAME OpenSkyscraper)

# Debugging definitions.
if (CMAKE_BUILD_TYPE MATCHES Debug)
	add_definitions("-DBUILD_DEBUG")
endif()

if (MSVC)
	add_definitions(-Dsnprintf=_snprintf)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()


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

# Include the ObjectiveLua headers.
include_directories(thirdparty/objectivelua/include)

# Include AStar-Search headers
include_directories(thirdparty/astar-search/)

# Lua
find_package(Lua51 REQUIRED)
include_directories(${LUA_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} ${LUA_LIBRARY})

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
	install(DIRECTORY ${SFML_LIBRARIES} DESTINATION ${CMAKE_BINARY_DIR}/${EXECUTABLE_NAME}.app/Contents/Frameworks)
endif ()

# CEGUI
#find_package(CEGUI REQUIRED opengl)
#include_directories(${CEGUI_INCLUDE_DIR})
#target_link_libraries(${EXECUTABLE_NAME} ${CEGUI_LIBRARY})

# libRocket
if (CMAKE_BUILD_TYPE MATCHES Debug)
	set(rocket_debug "debugger")
endif()
if (WIN32)
	set(ROCKETDIR "" CACHE PATH "libRocket directory")
endif ()
find_package(Rocket REQUIRED core controls ${rocket_debug})
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

# Add the PkgConfig package to find packages that CMake doesn't.
find_package(PkgConfig)

# libmspack for decompressing KWAJ files (SIMTOWER.EX_)
# http://www.cabextract.org.uk/libmspack/
pkg_check_modules(MSPACK libmspack)
if (MSPACK_FOUND)
	include_directories(${MSPACK_INCLUDE_DIRS})
	link_directories(${MSPACK_LIBRARY_DIRS})
	target_link_libraries(${EXECUTABLE_NAME} ${MSPACK_LIBRARIES})
	add_definitions(-DMSPACK)
endif (MSPACK_FOUND)
