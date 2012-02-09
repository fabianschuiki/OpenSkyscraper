#include <cassert>

#include "Application.h"
#include "DataManager.h"

using namespace OT;
using std::string;

DataManager::DataManager(Application * app)
:	app(app)
{
	assert(app != NULL && "DataManager requires an app");
}

void DataManager::init()
{
	LOG(INFO, "initializing data dirs");
	
	Path path = app->getPath();
	string name = path.name();
#ifdef __APPLE__
	dirs.push_back(Path("~/Library/Application Support").down(name));
	dirs.push_back(Path("/Library/Application Support").down(name));
	dirs.push_back(".");
#else
	dirs.push_back(Path("~").down(string(".")+name));
	dirs.push_back(Path("/usr/local/share").down(name));
	dirs.push_back(Path("/usr/share").down(name));
	dirs.push_back(path.up().down("data"));
#endif
	
	for (int i = 0; i < dirs.size(); i++)
		LOG(DEBUG, "  %s", dirs[i].c_str());
}

/** Returns a list of possible paths to the given resource. */
DataManager::Paths DataManager::paths(Path resource) const
{
	Paths p;
	for (Paths::iterator d = dirs.begin(); d != dirs.end(); d++)
		p.push_back(*d + resource);
	return p;
}
