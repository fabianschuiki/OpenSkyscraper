#pragma once
#include <ostream>

namespace OT
{
	class Logger
	{
	public:
		typedef enum {
			ERROR,
			IMPORTANT,
			INFO,
			DEBUG
		} LogLevel;
		
		Logger();
		
		LogLevel getLevel() const;
		void setLevel(LogLevel l);
		
		const std::string & getOutputPath() const;
		void setOutputPath(std::string op);
		
		void log(LogLevel level, const char * file, int line, const char * func, const char * fmt, ...);
		
	private:
		LogLevel level;
		
		std::string outputPath;
		FILE * outputFile;
	};
}

#define LOG(level, ...) App->logger.log(Logger::level, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
