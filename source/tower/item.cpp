#include "item.h"
#include "tower.h"
#include "../entity.h"


Item::Item(recti frame, Tower * tower) : frame(frame), tower(tower)
{
	state = StateNone;
	reset();
	tower->addItem(this);
}

void Item::reset()
{
	for (std::set<Entity *>::iterator e = entities.begin(); e != entities.end(); e++)
		delete *e;
	entities.clear();
	
	initialized = false;
	//tower->uninitializedItems.insert(this);
}
