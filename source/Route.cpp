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

bool Route::empty() const
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

bool Route::usesItem(Item::Item * item) const
{
	return usedItems.count(item);
}

int Route::score() const
{
	return cached_score;
}

void Route::updateScore()
{
	cached_score = 0;
	Node prev;
	for (std::vector<Node>::iterator nit = nodes.begin(); nit != nodes.end(); nit++) {
		Node & n = *nit;
		if (prev.item) {
			if (n.item->canHaulPeople()) cached_score += 10;
			cached_score += abs(n.toFloor - prev.toFloor) * 10;
			cached_score += n.item->getRect().distanceX(prev.item->getRect());
		}
		prev = n;
	}
}
