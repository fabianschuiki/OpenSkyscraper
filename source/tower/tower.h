#pragma once
#include <set>

class Item;
class Engine;


class Tower {
public:
	Engine * const engine;
	
	Tower(Engine * e);
	
	std::set<Item *> items;
	std::set<Item *> uninitializedItems;
};
