#pragma once
#include <set>

class Game;
class Item;
class Sprite;


class Tower {
public:
	Game * const game;
	
	Tower(Game * e);
	~Tower();
	
	void addItem(Item * i);
	void removeItem(Item * i);
	
private:
	std::set<Item *> items;
};
