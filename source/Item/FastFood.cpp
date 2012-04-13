#include "../Game.h"
#include "FastFood.h"

using namespace OT::Item;


void FastFood::init()
{
	Item::init();
	
	variant = rand() % 5;
	open = false;
	
	sprite.SetImage(App->bitmaps["simtower/fastfood"]);
	sprite.SetCenter(0, 24);
	addSprite(&sprite);
	
	defaultCeiling();
	
	updateSprite();
}

void FastFood::encodeXML(tinyxml2::XMLPrinter & xml)
{
	Item::encodeXML(xml);
	xml.PushAttribute("variant", variant);
}

void FastFood::decodeXML(tinyxml2::XMLElement & xml)
{
	Item::decodeXML(xml);
	variant = xml.IntAttribute("variant");
	updateSprite();
}

void FastFood::updateSprite()
{
	int index = 3;
	if (open) index = std::min<int>(ceil(people.size() / 5.0), 2);
	sprite.SetSubRect(sf::IntRect(index*128, variant*24, (index+1)*128, (variant+1)*24));
	sprite.Resize(128, 24);
}

void FastFood::advance(double dt)
{
	if (game->time.checkHour(10)) {
		open = true;
		customersToday = 0;
		updateSprite();
	}
	if (game->time.checkHour(21) && open) {
		open = false;
		population = customersToday;
		game->populationNeedsUpdate = true;
		updateSprite();
	}
}
