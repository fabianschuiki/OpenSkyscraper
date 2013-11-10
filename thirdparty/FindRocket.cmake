# Locates libRocket GUI library
#
# This module defines the following variables:
# - For each module X (core, controls, debugger)
#   - ROCKET_X_LIBRARY
#   - ROCKET_X_FOUND
# - ROCKET_LIBRARIES
# - ROCKET_FOUND
# - ROCKET_INCLUDE_DIR


set(ROCKETDIR "" CACHE PATH "libRocket directory")

set(FIND_ROCKET_PATHS
	${ROCKETDIR}
	#/usr/local
	#/usr
	/sw
	/opt/local
	/opt/csw
	/opt
)

# find the include directory
find_path(
	ROCKET_INCLUDE_DIR Rocket/Core.h
	PATH_SUFFIXES include
	HINTS ${FIND_ROCKET_PATHS}
	
)
if (ROCKET_INCLUDE_DIR)
	set(ROCKET_FOUND TRUE)
endif()

# find the components
foreach (component ${Rocket_FIND_COMPONENTS})
	# convert the component name to lowercase and capitalized versions
	string(TOUPPER ${component} component_upper)
	string(SUBSTRING ${component} 0  1 component_f)
	string(SUBSTRING ${component} 1 -1 component_r)
	string(TOUPPER ${component_f} component_f)
	string(TOLOWER ${component_r} component_r)
	set(component_cap ${component_f}${component_r})
	
	# try to find the library
	# debug library
    find_library(
		ROCKET_${component_upper}_LIBRARY_DEBUG
		NAMES Rocket${component_cap}_d
		PATH_SUFFIXES lib64 lib
		HINTS ${FIND_ROCKET_PATHS}
	)
				 
	# release library
	find_library(
		ROCKET_${component_upper}_LIBRARY_RELEASE
		NAMES Rocket${component_cap}
		PATH_SUFFIXES lib64 lib
		HINTS ${FIND_ROCKET_PATHS}
	)
	
	# check whether we've found the library
	if (ROCKET_${component_upper}_LIBRARY_DEBUG OR ROCKET_${component_upper}_LIBRARY_RELEASE)
		set(ROCKET_${component_upper}_FOUND TRUE)
		
		# if both are found, set ROCKET_XXX_LIBRARY to contain both
        if (ROCKET_${component_upper}_LIBRARY_DEBUG AND ROCKET_${component_upper}_LIBRARY_RELEASE)
            set(ROCKET_${component_upper}_LIBRARY debug     ${ROCKET_${component_upper}_LIBRARY_DEBUG}
                                                  optimized ${ROCKET_${component_upper}_LIBRARY_RELEASE})
        endif()

        # if only one debug/release variant is found, set the other to be equal to the found one
        if (ROCKET_${component_upper}_LIBRARY_DEBUG AND NOT ROCKET_${component_upper}_LIBRARY_RELEASE)
            # debug and not release
            set(ROCKET_${component_upper}_LIBRARY_RELEASE ${ROCKET_${component_upper}_LIBRARY_DEBUG})
            set(ROCKET_${component_upper}_LIBRARY         ${ROCKET_${component_upper}_LIBRARY_DEBUG})
        endif()
        if (ROCKET_${component_upper}_LIBRARY_RELEASE AND NOT ROCKET_${component_upper}_LIBRARY_DEBUG)
            # release and not debug
            set(ROCKET_${component_upper}_LIBRARY_DEBUG ${ROCKET_${component_upper}_LIBRARY_RELEASE})
            set(ROCKET_${component_upper}_LIBRARY       ${ROCKET_${component_upper}_LIBRARY_RELEASE})
        endif()
	else()
		set(ROCKET_FOUND FALSE)
		set(ROCKET_${component_upper}_FOUND FALSE)
		set(ROCKET_${component_upper}_LIBRARY "")
		set(FIND_ROCKET_MISSING "${FIND_ROCKET_MISSING} ROCKET_${component_upper}_LIBRARY")
	endif()
	
	# mark as advanced
    MARK_AS_ADVANCED(ROCKET_${component_upper}_LIBRARY
                     ROCKET_${component_upper}_LIBRARY_RELEASE
                     ROCKET_${component_upper}_LIBRARY_DEBUG)
	
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
	message("Found Rocket: ${ROCKET_LIBRARIES}")
endif()
