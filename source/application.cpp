#include "application.h"
#include <cassert>
#include <iostream>

using namespace OT;
using namespace std;

Application::Application(int argc, char * argv[])
{
	assert(argc >= 1 && "argv[0] is required");
	setPath(argv[0]);
	
	cout << "Application initialized" << endl
	     << "    path: " << getPath().str() << endl
	     << "    dir:  " << getDir().str() << endl;
}

Path Application::getPath() const { return path; }
Path Application::getDir()  const { return dir;  }

void Application::setPath(const Path & p)
{
	path = p;
	dir  = p.up();
}
