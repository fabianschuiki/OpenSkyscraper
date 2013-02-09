#include "Route.h"
#include "Item/Item.h"

using namespace OT;


Route::Route()
{
	clear();
}

void Route::clear()
{
	nodes.clear();
	cached_score   = 0;
	numStairs     = 0;
	numEscalators = 0;
	numElevators  = 0;
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
	
	if (item->isElevator())                 numElevators++;
	if (item->prototype->id == "stairs")    numStairs++;
	if (item->prototype->id == "escalator") numEscalators++;
}

int Route::score() const
{
	return cached_score;
}

void Route::updateScore(int score) {
	cached_score = score;
}
