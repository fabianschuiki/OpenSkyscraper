#pragma once
#include <set>
#include "../math/rect.h"

class Tower;
class Entity;


class Item {
public:
	const recti frame;
	Tower * const tower;
	
	enum State {
		StateNone = 0,
		StateConstructing,
		StateAlive,
	};
	
	Item(recti frame, Tower * tower);
	
private:
	std::set<Entity *> entities;
	State state;
	bool initialized;
	void reset();
};
