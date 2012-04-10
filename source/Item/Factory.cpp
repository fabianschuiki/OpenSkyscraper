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
}

void Factory::loadPrototypes()
{
	assert(prototypes.empty() && "prototypes mustn't be loaded multiple times");
	
	prototypes.push_back(FastFood::makePrototype());
	
	std::stringstream str;
	str << "loaded prototypes:";
	for (int i = 0; i < prototypes.size(); i++) {
		str << "\n" << prototypes[i]->desc();
	}
	LOG(DEBUG, str.str().c_str());
}