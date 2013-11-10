# Copyright (c) 2013 Fabian Schuiki, hoshi10
# Locate the mspack library of decompression routines.

set(MSPACKDIR "" CACHE PATH "mspack directory (optional)")

find_path(
	MSPACK_INCLUDE_DIRS mspack.h
	PATH_SUFFIXES include
	HINTS ${MSPACKDIR})
find_library(
	MSPACK_LIBRARIES
	NAMES mspack
	PATH_SUFFIXES lib lib64
	HINTS ${MSPACKDIR})

find_package(PackageHandleStandardArgs)
find_package_handle_standard_args(mspack DEFAULT_MSG MSPACK_LIBRARIES MSPACK_INCLUDE_DIRS)

mark_as_advanced(
	MSPACK_INCLUDE_DIRS
	MSPACK_LIBRARIES
)