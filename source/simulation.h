#pragma once
#include <queue>
#include <list>

class Game;
class Item;


class Simulation {
public:
	/** The game that owns this simulation core. */
	Game * const game;
	
	Simulation(Game * e);
	
	/** Advances the simulation by the given amount of time. */
	void advance(double dt);
	
private:
	struct Timer {
		double time;
		Item * item;
	};
	std::list<Timer> timers;
	
	void think(Item * item);
};
