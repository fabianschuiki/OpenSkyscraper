cmake_minimum_required(VERSION 2.6)
project(OpenSkyscraper)

set(src_dir  ${PROJECT_SOURCE_DIR})
set(data_dir ${PROJECT_SOURCE_DIR}/../data)


# Gather all the required files.
file(GLOB_RECURSE sources RELATIVE ${src_dir} ${src_dir}/*.cpp)
file(GLOB_RECURSE resources RELATIVE ${src_dir} ${data_dir}/*)

# Make sure the resources are copied to the right location.
foreach(r ${resources})
	get_filename_component(r_abs ${r} ABSOLUTE)
	get_filename_component(r_dir ${r_abs} PATH)
	get_filename_component(data_dir_abs ${data_dir} ABSOLUTE)
	file(RELATIVE_PATH r_rel ${data_dir_abs} ${r_dir})
	set_source_files_properties(${r} PROPERTIES MACOSX_PACKAGE_LOCATION Resources/${r_rel})
endforeach()

# For Mac OS X, copy the SFML frameworks to the destination.
set(frameworks
	../macosx/sfml/sfml-system.framework
	../macosx/sfml/sfml-window.framework
	../macosx/sfml/sfml-graphics.framework
)
set(FRAMEWORKS_DIR ${CMAKE_BINARY_DIR}/OpenSkyscraper.app/Contents/Frameworks)
install(DIRECTORY ${frameworks} DESTINATION ${FRAMEWORKS_DIR})


# OpenSkyscraper executable.
add_executable(
	OpenSkyscraper MACOSX_BUNDLE
	${sources}
	${resources}
)


# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#                                                                             #
#  Packages and Libraries                                                     #
#                                                                             #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

# Include the ObjectiveLua headers.
include_directories(${PROJECT_SOURCE_DIR}/../thirdparty/objectivelua/include/)

# Lua
find_package(Lua51 REQUIRED)
include_directories(${LUA_INCLUDE_DIR})
target_link_libraries(OpenSkyscraper ${LUA_LIBRARY})

# SFML
include_directories(../macosx/sfml/SFML.framework)
target_link_libraries(
	OpenSkyscraper
	"-F../macosx/sfml/"
	"-framework sfml-system"
	"-framework sfml-window"
	"-framework sfml-graphics"
)
