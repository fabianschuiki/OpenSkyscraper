#pragma once
#include "logger.h"
#include "path.h"

namespace OT
{
	class Application
	{
	public:
		Application(int argc, char * argv[]);
		
		Path getPath() const;
		Path getDir() const;
		
		Logger logger;
		
	private:
		Path path;
		Path dir;
		void setPath(const Path & p);
	};
	
	extern Application * App;
}
