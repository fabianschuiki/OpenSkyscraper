#include <cassert>
#include <cstdarg>
#include <cstring>
#include <string>

#include "Logger.h"
#include "Path.h"

using namespace OT;

void fputs_padded(FILE * f, const char * str, int padding)
{
	for (const char * s = str; *s != 0; s++) {
		fputc(*s, f);
		if (*s == '\n') {
			for (int i = 0; i < padding; i++)
				fputc(' ', f);
			fputs("| ", f);
		}
	}
}

#ifdef _WIN32
int vasprintf(char **strp, const char *fmt, va_list ap) {
	int buffer_size = 16;
	int size = 0;
	*strp = new char[buffer_size];
	size = vsnprintf(*strp, buffer_size, fmt, ap);
#ifdef _MSC_VER
	while(size < 0) {
		delete *strp;
		buffer_size *= 2;
		*strp = new char[buffer_size];
		size = vsnprintf(*strp, buffer_size, fmt, ap);
	}
	assert(size < buffer_size);
#else
	if(size >= buffer_size) {
		delete *strp;
		buffer_size = size + 1;
		*strp = new char[buffer_size];
		size = vsnprintf(*strp, buffer_size, fmt, ap);
		assert(size < buffer_size);
	}
#endif
	return size;
}
#endif

Logger::Logger()
{
	level = ERROR;
	outputFile = NULL;
}

Logger::LogLevel Logger::getLevel() const { return level; }
void Logger::setLevel(LogLevel l) { level = l; }

const std::string & Logger::getOutputPath() const { return outputPath; }
void Logger::setOutputPath(std::string op)
{
	if (outputPath != op) {
		outputPath = op;
		if (outputFile) {
			fclose(outputFile);
			outputFile = NULL;
		}
		log(DEBUG, __FILE__, __LINE__, __FUNCTION__, "redirecting log to %s", op.c_str());
		outputFile = fopen(outputPath.c_str(), "w");
		assert(outputFile != NULL && "unable to open output file");
	}
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
	const char * filename = strrchr(file, Path::SEPARATOR);
	if (filename)
		filename += 1;
	else
		filename = file;
	
	//Write to the console.
	FILE * f = (level == ERROR ? stderr : stdout);
#ifndef _WIN32
	int padding = fprintf(f, "\e[0;32m%s\e[0m.%i, \e[1;34m%s\e[0m: ", filename, line, func) - 2 - 22;
	if (level == ERROR)     fputs("\e[1;31m*** ", f);
	if (level == WARNING)   fputs("\e[1;33m", f);
	if (level == IMPORTANT) fputs("\e[1;35m", f);
	fputs_padded(f, str, padding);
	if (level == ERROR || level == WARNING || level == IMPORTANT) fputs("\e[0m", f);
#else
	HANDLE hConsole = (level == ERROR ? GetStdHandle(STD_ERROR_HANDLE) : GetStdHandle(STD_OUTPUT_HANDLE));
	CONSOLE_SCREEN_BUFFER_INFO *ConsoleInfo = new CONSOLE_SCREEN_BUFFER_INFO();
	GetConsoleScreenBufferInfo(hConsole, ConsoleInfo);
	WORD OriginalColors = ConsoleInfo->wAttributes;
	//int padding = fprintf(f, "%s.%i, %s: ", filename, line, func) - 2 - 22;
	SetConsoleTextAttribute(hConsole, 0x0a);
	int padding = fprintf(f, "%s", filename);
	SetConsoleTextAttribute(hConsole, OriginalColors);
	padding += fprintf(f, ".%i, ", line);
	SetConsoleTextAttribute(hConsole, 0x0b);
	padding += fprintf(f, "%s", filename);
	SetConsoleTextAttribute(hConsole, OriginalColors);
	padding += fprintf(f, ": ") - 2 - 22;
	if (level == ERROR) {
		SetConsoleTextAttribute(hConsole, 0x0c);
		fputs("*** ", f);
	}
	if (level == WARNING) {
		SetConsoleTextAttribute(hConsole, 0x0e);
		fputs("", f);
	}
	if (level == IMPORTANT) {
		SetConsoleTextAttribute(hConsole, 0x0d);
		fputs("", f);
	}
	fputs_padded(f, str, padding);
	if (level == ERROR || level == WARNING || level == IMPORTANT)
		SetConsoleTextAttribute(hConsole, OriginalColors);
#endif
	fputc('\n', f);
	
	//Write to the output file.
	if (outputFile) {
		fprintf(outputFile, "%s.%i, %s: ", filename, line, func);
		if (level == ERROR) fputs("*** ", outputFile);
		fputs_padded(outputFile, str, padding);
		fputc('\n', outputFile);
	}
	
	//Clean up.
	delete str;
}
