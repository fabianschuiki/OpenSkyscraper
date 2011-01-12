#include "external.h"

#ifndef PLATFORM_APPLE

#include "application.h"
vector<std::string> OSS::Core::Application::resourcesPaths()
{
	vector<std::string> paths;
	paths.push_back(".");
	paths.push_back("./resources");
	return paths;
}

#endif