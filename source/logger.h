#pragma once
#include <ostream>

namespace OT
{	
	class Logger
	{
	public:
		Logger();
		
		typedef enum {
			ERROR,
			IMPORTANT,
			MAIN,
			DEBUG
		} LogLevel;
		LogLevel level;
		
		void log(LogLevel level, const char * file, int line, const char * func, const char * fmt, ...);
	};
}

#define LOG(level, ...) App->logger.log(Logger::level, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
