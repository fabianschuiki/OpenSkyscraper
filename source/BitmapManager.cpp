#include "Application.h"
#include "BitmapManager.h"
#include "DataManager.h"

using namespace OT;


bool BitmapManager::load(Path name, sf::Image & dst)
{
	if (name.str().find(internal_path) == 0) {
		dst.SetSmooth(false);
		return true;
	} else {
		//Fetch the possible locations for this font.
		DataManager::Paths paths = app->data.paths(Path("bitmaps") + name);
		paths.push_back(name);
		
		//Try to load the font.
		bool success = false;
		for (int i = 0; i < paths.size() && !success; i++) {
			success = dst.LoadFromFile(paths[i].c_str());
		}
		
		//Return success.
		if (success) {
			LOG(DEBUG,   "loaded bitmap '%s'", name.c_str());
		} else {
			LOG(WARNING, "unable to find bitmap '%s'", name.c_str());
		}
		return success;
	}
}
