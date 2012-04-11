#include <cassert>
#include <string>
#include "Item.h"

using namespace OT::Item;
using std::string;


void Item::setPosition(int2 p)
{
	if (position != p) {
		position = p;
		SetPosition(p.x*8, p.y*32);
	}
}

void Item::addSprite(sf::Sprite * sprite)
{
	assert(sprite);
	sprites.insert(sprite);
}

void Item::removeSprite(sf::Sprite * sprite)
{
	assert(sprite);
	sprites.erase(sprite);
}

void Item::Render(sf::RenderTarget & target) const
{
	for (SpriteSet::iterator s = sprites.begin(); s != sprites.end(); s++) {
		target.Draw(**s);
	}
}

void Item::encodeXML(tinyxml2::XMLPrinter & xml)
{
	xml.PushAttribute("type", prototype->id.c_str());
	xml.PushAttribute("x", position.x);
	xml.PushAttribute("y", position.y);
}

void Item::decodeXML(tinyxml2::XMLElement & xml)
{
	setPosition(int2(xml.IntAttribute("x"), xml.IntAttribute("y")));
}
