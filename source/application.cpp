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
	
	//Special debug defaults.
#ifdef BUILD_DEBUG
	logger.setLevel(Logger::DEBUG);
	char logname[128];
	snprintf(logname, 128, "debug-%li.log", (long int)time(NULL));
	logger.setOutputPath(dir.down(logname));
#endif
	
	//Parse command line arguments.
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
		"constructed\n"
		"    path = %s\n"
		"    dir  = %s",
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

/** Runs the application. */
int Application::run()
{
	running = true;
	exitCode = 0;
	
	init();
	loop();
	cleanup();
	
	running = false;
	
	if (exitCode < 0) {
		LOG(ERROR, "exitCode = %i", exitCode);
	} else {
		LOG(INFO,  "exitCode = %i", exitCode);
	}
	
	return exitCode;
}

void Application::init()
{
	videoMode.Width        = 800;
	videoMode.Height       = 600;
	videoMode.BitsPerPixel = 32;
	
	window.Create(videoMode, "OpenSkyscraper SFML");
}

void Application::loop()
{
}

void Application::cleanup()
{
	
}
