#include "tower.h"
#include "item.h"


Tower::Tower(Game * e) : game(e)
{
}

Tower::~Tower()
{
}



void Tower::addItem(Item * i)
{
	items.insert(i);
}

void Tower::removeItem(Item * i)
{
	if (i->tower != this)
		return;
	items.erase(i);
}
