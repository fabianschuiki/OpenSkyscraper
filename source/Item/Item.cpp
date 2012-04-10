#include <cassert>
#include "Item.h"

using namespace OT::Item;


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
