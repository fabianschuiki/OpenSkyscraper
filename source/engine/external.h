#ifndef OSS_ENGINE_EXTERNAL_H
#define OSS_ENGINE_EXTERNAL_H

#include "../core/headers.h"
#include "types.h"

// Mac OS X
#if defined(__APPLE__) || defined(__MACH__)
	#define PLATFORM_APPLE
	#include <OpenAL/al.h>
	#include "alc.h" //required since alc.h in OS X uses typedef'd void for empty param list -> C++ sucks!
	#include <OpenGL/gl.h>
#endif

// GNU/Linux
#if defined(linux) || defined(__linux)
	#define PLATFORM_LINUX
	#include <AL/al.h>
	#include <AL/alc.h>
	#include <GL/gl.h>
#endif

#if defined(_WIN32)
	#define PLATFORM_WINDOWS
	#error Not ported to Windows yet. 
	// Oh god you poor porters.
	// If you are going to do the unspeakable, please check out platformspecific.cpp.
#endif

#include <SDL/SDL.h>
#include <IL/il.h>

#endif
