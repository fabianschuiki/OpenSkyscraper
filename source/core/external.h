#ifndef OSS_CORE_EXTERNAL_H
#define OSS_CORE_EXTERNAL_H

#include "../base/headers.h"
#include "../math/headers.h"

//Unfortunately there's still some DOSism around
//TODO: Find a better solution to this problem. For example, using Unix-style paths everywhere, con-
//verting them to Windows format where needed.
#ifdef __WIN32__
#define PATH_SEGMENTER "\\"
#else
#define PATH_SEGMENTER "/"
#endif

#endif
