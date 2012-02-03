#pragma once
#include "path.h"

namespace OT
{
	class Application
	{
	public:
		Application(int argc, char * argv[]);
		
		Path getPath() const;
		Path getDir() const;
		
	private:
		Path path;
		Path dir;
		void setPath(const Path & p);
	};
}
