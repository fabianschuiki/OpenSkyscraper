cmake_minimum_required(VERSION 2.6)
project(OpenSkyscraper)

# Executable name.
set(EXECUTABLE_NAME OpenSkyscraper)

# Important project directories.
set(src_dir  ${PROJECT_SOURCE_DIR})
set(data_dir ${PROJECT_SOURCE_DIR}/../data)


# Gather the source files.
file(GLOB_RECURSE sources RELATIVE ${src_dir} ${src_dir}/*.cpp)


###  PLATFORM-SPECIFIC STUFF
###  =======================

# Mac OS X
if (APPLE)
	# Find the paths of all resources.
	file(GLOB_RECURSE resources RELATIVE ${src_dir} ${data_dir}/*)

	# Make sure the resources are copied to the right bundle.
	foreach(r ${resources})
		get_filename_component(r_abs ${r} ABSOLUTE)
		get_filename_component(r_dir ${r_abs} PATH)
		get_filename_component(data_dir_abs ${data_dir} ABSOLUTE)
		file(RELATIVE_PATH r_rel ${data_dir_abs} ${r_dir})
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
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../thirdparty" ${CMAKE_MODULE_PATH})

# Include the ObjectiveLua headers.
include_directories(${PROJECT_SOURCE_DIR}/../thirdparty/objectivelua/include/)

# Lua
find_package(Lua51 REQUIRED)
include_directories(${LUA_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} ${LUA_LIBRARY})

# SFML
find_package(SFML REQUIRED system window graphics)
include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})

# Copy the SFML into the application bundle on Mac OS X.
if (APPLE)
	install(DIRECTORY ${SFML_LIBRARIES} DESTINATION ${CMAKE_BINARY_DIR}/${EXECUTABLE_NAME}.app/Contents/Frameworks)
endif ()
