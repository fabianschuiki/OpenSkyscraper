#include "../Game.h"
#include "../Math/Rand.h"
#include "Metro.h"

using namespace OT;
using namespace OT::Item;


Metro::~Metro()
{
}

void Metro::init()
{
	Item::init();

	open = true;
	trainPresent = true;

	station.SetImage(App->bitmaps["simtower/metro/station"]);
	station.setOrigin(0, 96);
	platform.SetImage(App->bitmaps["simtower/metro/station"]);
	platform.setOrigin(0, 30);
	addSprite(&station);
	addSprite(&platform);
	spriteNeedsUpdate = true;

	assert(game->metroStation == NULL);
	game->metroStation = this;

	updateSprite();
}

void Metro::encodeXML(tinyxml2::XMLPrinter & xml)
{
	Item::encodeXML(xml);
	xml.PushAttribute("open", open);
	xml.PushAttribute("trainPresent", trainPresent);
}

void Metro::decodeXML(tinyxml2::XMLElement & xml)
{
	Item::decodeXML(xml);
	open = xml.BoolAttribute("open");
	trainPresent = xml.BoolAttribute("trainPresent");
	updateSprite();
}

void Metro::updateSprite()
{
	spriteNeedsUpdate = false;
	int stationIndex = 2, platformIndex = 2;
	if (open) {
		stationIndex = 1;
		platformIndex = (trainPresent ? 0 : 1);
	}

	station.setTextureRect(sf::IntRect(stationIndex*240, 0, 240, 66));
	platform.setTextureRect(sf::IntRect(platformIndex*240, 66, 240, 96));
}

void Metro::advance(double dt)
{
	//Open
	if (game->time.checkHour(7)) {
		open = true;
		spriteNeedsUpdate = true;
	}

	//Close
	if (game->time.checkHour(23) && open) {
		open = false;
		spriteNeedsUpdate = true;
	}

	if (spriteNeedsUpdate) updateSprite();
}
