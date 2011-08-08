#ifndef OSS_ENGINE_PLATFORM_H
#define OSS_ENGINE_PLATFORM_H

// Mac OS X
#if defined(__APPLE__) || defined(__MACH__)
	#define PLATFORM_APPLE
	#include "apple.h"
#endif

// GNU/Linux
#if defined(linux) || defined(__linux) || defined (__FreeBSD__)
	#define PLATFORM_LINUX
	#include "linux.h"
#endif

// Windows
#if defined(_WIN32)
	// I'm sure _WIN32 is also defined when compiling for 64bit :)
	#define PLATFORM_WINDOWS
	#error Not ported to Windows yet. 
	// Oh god you poor porters.
	// If you are going to do the unspeakable, please check out platformspecific.cpp.
#endif

//Common to all platforms
#include "common.h"

#endif
