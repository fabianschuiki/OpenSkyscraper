#include "simulation.h"


Simulation::Simulation(Engine * e) : engine(e)
{
}


void Simulation::advance(double dt)
{
	double time = 0;
	while (!timers.empty() && timers.front().time <= time) {
		think(timers.front().item);
		timers.pop_front();
	}
}

void Simulation::think(Item * item)
{
}
