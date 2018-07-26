#include <cassert>
#include "Item/Item.h"
#include "Person.h"

using namespace OT;


Person::Person(Game * game, Type type)
:	GameObject(game),
	journey(this)
{
	at     = NULL;
	type   = kMan;
	stress = 0;
}

Person::~Person()
{
	if (at) {
		LOG(DEBUG, "forcefully removed from %s", at->desc().c_str());
		at->removePerson(this);
	}
}

Person::Journey::Journey(Person * p)
:	person(p)
{
	fromFloor = 0;
	item = NULL;
	toFloor = 0;
}

void Person::Journey::set(const Route & r)
{
	while (!nodes.empty()) nodes.pop();
	for (std::vector<Route::Node>::const_iterator nit = r.nodes.begin(); nit != r.nodes.end(); nit++) {
		nodes.push(*nit);
	}
	toFloor = nodes.front().toFloor;
	next();
}

void Person::Journey::next()
{
	//Remove the person from where he/she is currently at.
	if (person->at) person->at->removePerson(person);
	
	//Keep the current floor around.
	fromFloor = toFloor;
	
	//Jump to next node.
	assert(!nodes.empty());
	nodes.pop();
	assert(!nodes.empty());
	
	//Add the person to the node's item.
	item    = nodes.front().item;
	toFloor = nodes.front().toFloor;
	assert(item);
	item->addPerson(person);
}

int Person::getWidth()
{
	return (type >= kHousekeeper ? 16 : 8);
}
