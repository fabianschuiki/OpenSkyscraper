#include "logger.h"
#include <cstdarg>
#include <cstdio>
#include <cstring>

using namespace OT;

Logger::Logger()
{
#ifdef BUILD_DEBUG
	level = DEBUG;
#else
	level = ERROR;
#endif
}

void Logger::log(LogLevel level, const char * file, int line, const char * func, const char * fmt, ...)
{
	//Ignore certain logs that are above the set maximum level.
	if (level > this->level)
		return;
	
	//Format the string.
	va_list args;
	va_start(args, fmt);
	char * str;
	vasprintf(&str, fmt, args);
	va_end(args);
	
	//Fetch the filename.
	const char * filename = strrchr(file, '/');
	if (filename)
		filename += 1;
	else
		filename = file;
	
	//Write to the console.
	FILE * f = (level == ERROR ? stderr : stdout);
	int padding = fprintf(f, "\e[0;32m%s\e[0m.%4i, \e[1;34m%s\e[0m: ", filename, line, func) - 2 - 22;
	if (level == ERROR) fputs("\e[1;31m*** ", f);
	if (level == IMPORTANT) fputs("\e[1;35m", f);
	for (char * s = str; *s != 0; s++) {
		fputc(*s, f);
		if (*s == '\n') {
			for (int i = 0; i < padding; i++)
				fputc(' ', f);
			fputs("| ", f);
		}
	}
	if (level == ERROR || level == IMPORTANT) fputs("\e[0m", f);
	fputc('\n', f);
	
	//Clean up.
	delete str;
}
