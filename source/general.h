/**
 * Standard C/C++ libraries and includes
 */

//Basics
#include <math.h>
#include <assert.h>
#include <dirent.h>

//C++ IOstream library
#include <ios>
#include <iostream>
#include <fstream>
#include <sstream>

//C++ Strings library
#include <string>

//C++ STL Containers
#include <list>
#include <vector>
#include <typeinfo>
#include <stack>
#include <queue>
#include <set>
#include <map>
#include <bitset>


/*
 * Third-party and non-C/C++ libraries
 */

//SDL
#ifdef __APPLE__
#include <SDL/SDL.h>
#endif

//libpng
#include <png.h>

//DevIL
#include <IL/il.h>

//OpenGL
#ifdef __APPLE__
#include <OpenGL/gl.h>
#endif

//OpenAL
#ifdef __APPLE__
#include <OpenAL/al.h>
//#include <OpenAL/alc.h>
#include "os/macosx/alc.h" //required since alc.h in OS X uses typedef'd void for empty param list -> C++ sucks!
#endif

//AntTweakBar
//#include <AntTweakBar.h>


/**
 * Common OpenSkyscraper headers
 */
#include "types.h"
#include "macros.h"
