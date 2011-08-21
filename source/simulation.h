#pragma once
#include <queue>
#include <list>

class Engine;
class Item;


class Simulation {
public:
	Engine * const engine;
	Simulation(Engine * e);
	
	void advance(double dt);
	
private:
	struct Timer {
		double time;
		Item * item;
	};
	std::list<Timer> timers;
	
	void think(Item * item);
};
