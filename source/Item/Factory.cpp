#include <cassert>
#include <sstream>
#include "../Application.h"
#include "Elevator/Express.h"
#include "Elevator/Service.h"
#include "Elevator/Standard.h"
#include "Escalator.h"
#include "Factory.h"
#include "FastFood.h"
#include "Lobby.h"
#include "Stairs.h"

using namespace OT;
using namespace Item;


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
	register_item(Elevator::Express);
	register_item(Elevator::Service);
	register_item(Elevator::Standard);
	register_item(FastFood);
	
	for (int i = 0; i < prototypes.size(); i++) {
		prototypesById[prototypes[i]->id] = prototypes[i];
	}
}

OT::Item::Item * Factory::make(AbstractPrototype * prototype, int2 position)
{
	assert(prototype);
	Item * item = prototype->make(game);
	item->setPosition(position);
	item->init();
	return item;
}

OT::Item::Item * Factory::make(std::string prototypeID, int2 position)
{
	AbstractPrototype * prototype = prototypesById[prototypeID];
	return make(prototype, position);
}

OT::Item::Item * Factory::make(tinyxml2::XMLElement & xml)
{
	int2 position(xml.IntAttribute("x"), xml.IntAttribute("y"));
	Item * item = make(xml.Attribute("type"), position);
	assert(item);
	item->decodeXML(xml);
	return item;
}
