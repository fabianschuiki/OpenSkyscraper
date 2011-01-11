#ifndef OSS_ENGINE_EXTERNAL_H
#define OSS_ENGINE_EXTERNAL_H

#include "../core/headers.h"
#include "types.h"

// Mac OS X
#if defined(__APPLE__) || defined(__MACH__)
	#include <OpenAL/al.h>
	#include "alc.h" //required since alc.h in OS X uses typedef'd void for empty param list -> C++ sucks!
	#include <OpenGL/gl.h>
#endif

// GNU/Linux
#if defined(linux) || defined(__linux)
	#include <AL/al.h>
	#include <AL/alc.h>
	#include <GL/gl.h>
#endif

#if defined(_WIN32)
	// Oh god you poor porters.
	#error Not ported to Windows yet.
#endif

#include <SDL/SDL.h>
#include <IL/il.h>

#endif
