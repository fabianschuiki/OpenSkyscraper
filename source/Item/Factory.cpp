#include <cassert>
#include <sstream>
#include "../Application.h"
#include "Factory.h"
#include "FastFood.h"

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
	
	prototypes.push_back(FastFood::makePrototype());
	
	for (int i = 0; i < prototypes.size(); i++) {
		prototypesById[prototypes[i]->id] = prototypes[i];
	}
}

Item * Factory::make(AbstractPrototype * prototype)
{
	assert(prototype);
	return prototype->make(game);
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
