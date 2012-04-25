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
	string name = /*path.name()*/"OpenSkyscraper";
#ifdef __APPLE__
#ifdef BUILD_DEBUG
	//TODO: there's an issue with climbing up paths that go like ../../, since path.up() will first erase these ../ segments, instead of directly appending to them.
	//dirs.push_back(path.up(0).down("data"));
	dirs.push_back(path.up(6).down("data"));
#endif
	dirs.push_back(Path("~/Library/Application Support").down(name));
	dirs.push_back(Path("/Library/Application Support").down(name));
	dirs.push_back(".");
#else
#ifdef BUILD_DEBUG
	dirs.push_back(path.up(2).down("data"));
#endif
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
	for (Paths::const_iterator d = dirs.begin(); d != dirs.end(); d++)
		p.push_back(*d + resource);
	return p;
}
