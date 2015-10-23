#include <cassert>
#include <sys/stat.h>

#include "Application.h"
#include "DataManager.h"

using namespace OT;
using std::string;

bool DirectoryExists(const char * path)
{
	struct stat st;
	if(stat(path, &st) == 0 && (st.st_mode & S_IFMT) == S_IFDIR)
		return true;
	return false;
}

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
# ifdef BUILD_DEBUG
	//TODO: there's an issue with climbing up paths that go like ../../, since path.up() will first erase these ../ segments, instead of directly appending to them.
	//dirs.push_back(path.up(0).down("data"));
	dirs.push_back(Path("data"));
	dirs.push_back(path.up(1).down("Resources"));
# endif
	dirs.push_back(Path("~/Library/Application Support/OpenSkyscraper"));
	dirs.push_back(Path("/Library/Application Support/OpenSkyscraper"));
	dirs.push_back(".");
#else
# ifdef BUILD_DEBUG
	dirs.push_back(path.up(2).down("data"));
# endif
	dirs.push_back(path.down("data"));
	dirs.push_back(path.up().down("data"));
# ifndef _WIN32
	dirs.push_back(Path("~").down(".openskyscraper"));
	dirs.push_back(Path("/usr/local/share/openskyscraper"));
	dirs.push_back(Path("/usr/share/openskyscraper"));
# endif
#endif

	Paths existing;
	for (Paths::const_iterator it = dirs.begin(); it != dirs.end(); ++it) {
		bool exists = DirectoryExists(it->c_str());
		LOG(DEBUG, (exists ? "  %s" : "  %s (not found)"), it->c_str());
		if (exists)
			existing.push_back(*it);
	}
	std::swap(dirs, existing);
}

/** Returns a list of possible paths to the given resource. */
DataManager::Paths DataManager::paths(Path resource) const
{
	Paths p;
	for (Paths::const_iterator d = dirs.begin(); d != dirs.end(); d++)
		p.push_back(*d + resource);
	return p;
}
