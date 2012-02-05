#include "application.h"
#include <cassert>
#include <cstring>

using namespace OT;
using namespace std;

Application * OT::App = NULL;

Application::Application(int argc, char * argv[])
{
	assert(App == NULL && "Application initialized multiple times");
	App = this;
	
	assert(argc >= 1 && "argv[0] is required");
	setPath(argv[0]);
	
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--debug") == 0)
			logger.level = Logger::DEBUG;
	}
	
	LOG(DEBUG,
		"initialized\n"
		"    path: %s\n"
		"    dir:  %s",
		getPath().str().c_str(), getDir().str().c_str()
	);
	LOG(IMPORTANT, "ready");
}

Path Application::getPath() const { return path; }
Path Application::getDir()  const { return dir;  }

void Application::setPath(const Path & p)
{
	path = p;
	dir  = p.up();
}
