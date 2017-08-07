#include "Application.h"
#include "SoundManager.h"
#include "DataManager.h"

using namespace OT;


bool SoundManager::load(Path name, sf::SoundBuffer & dst)
{
	if (name.str().find(internal_path) == 0) {
		return true;
	} else {
		DataManager::Paths paths = app->data.paths(Path("sounds") + name);
		paths.push_back(name);
		
		bool success = false;
		for (int i = 0; i < paths.size() && !success; i++) {
			success = dst.loadFromFile(paths[i].c_str());
		}
		
		if (success) {
			LOG(DEBUG,   "loaded sound '%s'", name.c_str());
		} else {
			LOG(WARNING, "unable to find sound '%s'", name.c_str());
		}
		return success;
	}
}
