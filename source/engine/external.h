#ifndef OSS_ENGINE_EXTERNAL_H
#define OSS_ENGINE_EXTERNAL_H

#include "../core/headers.h"
#include "types.h"

//OpenAL
#ifdef __APPLE__
#include <OpenAL/al.h>
//#include <OpenAL/alc.h>
#include "alc.h" //required since alc.h in OS X uses typedef'd void for empty param list -> C++ sucks!
#endif

//OpenGL
#ifdef __APPLE__
#include <OpenGL/gl.h>
#endif

//SDL
#ifdef __APPLE__
#include <SDL/SDL.h>
#endif

//DevIL
#ifdef __APPLE__
#include <IL/il.h>
#endif

#endif
