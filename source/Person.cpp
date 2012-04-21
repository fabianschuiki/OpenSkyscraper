#include <cassert>
#include "Item/Item.h"
#include "Person.h"

using namespace OT;


Person::Person(Game * game)
:	GameObject(game),
	journey(this)
{
	at = NULL;
}

Person::~Person()
{
	if (at) {
		LOG(DEBUG, "forcefully removed from %s", at->desc().c_str());
		at->removePerson(this);
	}
}

void Person::Journey::set(const Route & r)
{
	while (!nodes.empty()) nodes.pop();
	for (std::vector<Route::Node>::const_iterator nit = r.nodes.begin(); nit != r.nodes.end(); nit++) {
		nodes.push(*nit);
	}
	next();
}

Item::Item * Person::Journey::item()
{
	if (nodes.empty()) return NULL;
	return nodes.front().item;
}

int Person::Journey::toFloor()
{
	if (nodes.empty()) return 0;
	return nodes.front().toFloor;
}

void Person::Journey::next()
{
	//Remove the person from where he/she is currently at.
	if (person->at) person->at->removePerson(person);
	
	//Jump to next node.
	assert(!nodes.empty());
	nodes.pop();
	assert(!nodes.empty());
	
	//Add the person to the node's item.
	Item::Item * i = item();
	if (i) i->addPerson(person);
}
