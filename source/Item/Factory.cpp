#include <cassert>
#include <sstream>
#include "../Application.h"
#include "Escalator.h"
#include "Factory.h"
#include "FastFood.h"
#include "Lobby.h"
#include "Stairs.h"
#include "StandardElevator.h"

using namespace OT::Item;


Factory::~Factory()
{
	for (int i = 0; i < prototypes.size(); i++) {
		delete prototypes[i];
	}
	prototypes.clear();
	prototypesById.clear();
}

void Factory::loadPrototypes()
{
	assert(prototypes.empty() && "prototypes mustn't be loaded multiple times");
	
	#define register_item(cls) prototypes.push_back(cls::makePrototype());
	register_item(Lobby);
	register_item(Stairs);
	register_item(Escalator);
	register_item(StandardElevator);
	register_item(FastFood);
	
	for (int i = 0; i < prototypes.size(); i++) {
		prototypesById[prototypes[i]->id] = prototypes[i];
	}
}

Item * Factory::make(AbstractPrototype * prototype)
{
	assert(prototype);
	Item * item = prototype->make(game);
	item->init();
	return item;
}

Item * Factory::make(std::string prototypeID)
{
	AbstractPrototype * prototype = prototypesById[prototypeID];
	return make(prototype);
}

Item * Factory::make(tinyxml2::XMLElement & xml)
{
	Item * item = make(xml.Attribute("type"));
	assert(item);
	item->decodeXML(xml);
	return item;
}
