#include <cassert>
#include "../Game.h"
#include "Floor.h"

using namespace OT;
using namespace Item;


Floor::~Floor()
{
	assert(position.y != 0);
}

void Floor::init()
{
	Item::init();
	
	layer = -1;
	
	background.SetImage(App->bitmaps["simtower/floor"]);
	background.SetCenter(0, 36);
	background.SetSubRect(sf::IntRect(0, 0, 8, 36));
	
	updateSprite();
}

void Floor::encodeXML(tinyxml2::XMLPrinter & xml)
{
	Item::encodeXML(xml);
	xml.PushAttribute("width", size.x);
}

void Floor::decodeXML(tinyxml2::XMLElement & xml)
{
	Item::decodeXML(xml);
	size.x = xml.IntAttribute("width");
	updateSprite();
}

void Floor::updateSprite()
{
	background.Resize(GetSize().x, 36);
}

void Floor::Render(sf::RenderTarget & target) const
{
	target.Draw(background);
	game->drawnSprites++;
	Item::Render(target);
}
