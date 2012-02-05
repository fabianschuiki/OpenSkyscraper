#include "application.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>

using namespace OT;
using namespace std;

Application * OT::App = NULL;

Application::Application(int argc, char * argv[])
{
	assert(App == NULL && "Application initialized multiple times");
	App = this;
	
	assert(argc >= 1 && "argv[0] is required");
	setPath(argv[0]);
	
#ifdef BUILD_DEBUG
	logger.setLevel(Logger::DEBUG);
	char logname[128];
	snprintf(logname, 128, "debug-%li.log", (long int)time(NULL));
	logger.setOutputPath(dir.down(logname));
#endif
	
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--debug") == 0) {
			logger.setLevel(Logger::DEBUG);
		}
		if (strcmp(argv[i], "--log") == 0) {
			assert(i+1 < argc && "--log is missing path");
			logger.setOutputPath(argv[i++]);
		}
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
