#include "Application.h"
#include "BitmapManager.h"
#include "DataManager.h"

using namespace OT;


bool BitmapManager::load(Path name, sf::Image & dst)
{
	//At the moment we only disable image smoothing to get pixel-accurate rendering.
	//TODO: move the SimTower bitmaps into a "simtower" subdirectory. This way non-SimTower bitmaps can be loaded like fonts.
	dst.SetSmooth(false);
	return true;
}
