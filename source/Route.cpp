#include "Route.h"
#include "Item/Item.h"

using namespace OT;


Route::Route()
{
	cached_score = 0;
}

void Route::clear()
{
	nodes.clear();
	usedItems.clear();
	cached_score = 0;
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
	n.item = item;
	n.toFloor = floor;
	nodes.push_back(n);
	usedItems.insert(item);
	updateScore();
}

bool Route::usesItem(Item::Item * item)
{
	return usedItems.count(item);
}

int Route::score()
{
	return cached_score;
}

void Route::updateScore()
{
	cached_score = 0;
	Node prev;
	for (std::vector<Node>::iterator nit = nodes.begin(); nit != nodes.end(); nit++) {
		Node & n = *nit;
		if (!prev.item) {
			prev = n;
		} else {
			cached_score += 10 + abs(n.toFloor - prev.toFloor);
		}
	}
}
