#include <iostream>
#include "game.h"
#include "sprite.h"


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

void Game::addEntity(Entity * e)
{
	std::cout << "game: adding entity " << e << "\n";
	space.addEntity(e);
}

void Game::removeEntity(Entity * e)
{
	std::cout << "game: removing entity " << e << "\n";
	space.removeEntity(e);
}
