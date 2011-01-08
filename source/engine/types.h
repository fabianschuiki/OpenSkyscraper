#ifndef OSS_TYPES_H
#define OSS_TYPES_H

#include "general.h"

//Unfortunately there's still some DOSism around
#ifdef __WIN32__
#define PATH_SEGMENTER "\\"
#else
#define PATH_SEGMENTER "/"
#endif

//Colors
namespace OSS {
	typedef union {
		struct {
			double r;
			double g;
			double b;
		} c;
		double v[3];
	} color3d;
	
	typedef union {
		struct {
			double r;
			double g;
			double b;
			double a;
		} c;
		double v[4];
	} color4d;
}

//... various type declarations

#endif
