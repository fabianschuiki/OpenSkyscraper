# Locates libRocket GUI library
#
# This module defines the following variables:
# - For each module X (core, controls, debugger)
#   - ROCKET_X_LIBRARY
#   - ROCKET_X_FOUND
# - ROCKET_LIBRARIES
# - ROCKET_FOUND
# - ROCKET_INCLUDE_DIR

set(FIND_ROCKET_PATHS
	/usr/local
	/usr
	/sw
	/opt/local
	/opt/csw
	/opt
)

# find the include directory
find_path(
	ROCKET_INCLUDE_DIR Rocket/Core.h
	PATH_SUFFIXES include
	PATHS ${FIND_ROCKET_PATHS}
	
)

# find the components
set(ROCKET_FOUND TRUE)
foreach (component ${Rocket_FIND_COMPONENTS})
	# convert the component name to lowercase and capitalized versions
	string(TOUPPER ${component} component_upper)
	string(SUBSTRING ${component} 0  1 component_f)
	string(SUBSTRING ${component} 1 -1 component_r)
	string(TOUPPER ${component_f} component_f)
	string(TOLOWER ${component_r} component_r)
	set(component_cap ${component_f}${component_r})
	
	# try to find the library
	find_library(
		ROCKET_${component_upper}_LIBRARY
		NAMES Rocket${component_cap}
		PATH_SUFFIXES lib64 lib
		PATHS ${FIND_ROCKET_PATHS}
	)
	
	# check whether we've found the library
	if (ROCKET_${component_upper}_LIBRARY)
		set(ROCKET_${component_upper}_FOUND TRUE)
	else()
		set(ROCKET_FOUND FALSE)
		set(ROCKET_${component_upper}_FOUND FALSE)
		set(ROCKET_${component_upper}_LIBRARY "")
		set(FIND_ROCKET_MISSING "${FIND_ROCKET_MISSING} ROCKET_${component_upper}_LIBRARY")
	endif()
	
	# add to the list of libraries
	set(ROCKET_LIBRARIES ${ROCKET_LIBRARIES} "${ROCKET_${component_upper}_LIBRARY}")
endforeach()

# handle errors
if (NOT ROCKET_FOUND)
	set(FIND_ROCKET_ERROR "Could NOT find Rocket (missing: ${FIND_ROCKET_MISSING})")
	if (Rocket_FIND_REQUIRED)
		message(FATAL_ERROR ${FIND_ROCKET_ERROR})
	elseif (NOT Rocket_FIND_QUIETLY)
		message("${FIND_ROCKET_ERROR}")
	endif()
else()
	message("Found Rocket: ${ROCKET_INCLUDE_DIR}")
endif()
