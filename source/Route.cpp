#include "Route.h"
#include "Item/Item.h"

using namespace OT;


void Route::clear()
{
	nodes.clear();
	usedItems.clear();
}

bool Route::empty()
{
	return nodes.empty();
}

void Route::add(Item::Item * item)
{
	add(item, item->position.y);
}

void Route::add(Item::Item * item, int floor)
{
	Node n;
	n.item  = item;
	n.floor = floor;
	nodes.push_back(n);
	usedItems.insert(item);
}

bool Route::usesItem(Item::Item * item)
{
	return usedItems.count(item);
}
