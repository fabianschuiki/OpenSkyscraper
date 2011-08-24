#include <iostream>
#include "game.h"


Game::Game() : simulation(this), animation(this), package(this)
{
	//Install the class stuff for the Lua state.
	LuaClass::install(lua);
}

void Game::advance(double dt)
{
	simulation.advance(dt);
	//animation.advance(dt);
}
