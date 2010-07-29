#ifndef OSS_TYPES_H
#define OSS_TYPES_H

#include "general.h"

//General math classes and types
#include "math/math.h"

//Unfortunately there's still some DOSism around
#ifdef __WIN32__
#define PATH_SEGMENTER "\\"
#else
#define PATH_SEGMENTER "/"
#endif

//... various type declarations

#endif
