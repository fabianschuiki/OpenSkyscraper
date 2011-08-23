#include <iostream>
#include "game.h"


Game::Game() : simulation(this), animation(this), package(this)
{
}

void Game::advance(double dt)
{
	simulation.advance(dt);
	//animation.advance(dt);
}
