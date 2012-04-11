#include "FastFood.h"

using namespace OT::Item;


void FastFood::init()
{
	Item::init();
	
	variant = rand() % 5;
	
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
	sprite.SetSubRect(sf::IntRect(128*2, variant*24, 128*3, (variant+1)*24));
	sprite.Resize(128, 24);
}
