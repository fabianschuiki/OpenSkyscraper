#pragma once
#include <set>

class Item;
class Game;


class Tower {
public:
	Game * const game;
	
	Tower(Game * e);
	
	std::set<Item *> items;
	std::set<Item *> uninitializedItems;
};
