#include <cassert>
#include "Item.h"

using namespace OT::Item;


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
