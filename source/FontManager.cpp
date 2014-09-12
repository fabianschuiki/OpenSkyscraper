#include "Application.h"
#include "DataManager.h"
#include "FontManager.h"

using namespace OT;


bool FontManager::load(Path name, sf::Font & dst)
{
	//Fetch the possible locations for this font.
	DataManager::Paths paths = app->data.paths(Path("fonts") + name);
	
	//Try to load the font.
	bool success = false;
	for (int i = 0; i < paths.size() && !success; i++) {
		success = dst.loadFromFile(paths[i].c_str());
	}
	
	//Return success.
	if (success) {
		LOG(DEBUG,   "loaded font '%s'", name.c_str());
	} else {
		LOG(WARNING, "unable to find font '%s'", name.c_str());
	}
	return success;
}

bool FontManager::loadIntoRocket(Path name)
{
	DataManager::Paths paths = app->data.paths(Path("fonts") + name);
	for (DataManager::Paths::iterator p = paths.begin(); p != paths.end(); p++) {
		if (Rocket::Core::FontDatabase::LoadFontFace((*p).c_str())) return true;
	}
	LOG(WARNING, "unable to find font '&s'", name.c_str());
	return false;
}
