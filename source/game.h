#pragma once
#include "animation.h"
#include "lua.h"
#include "packages/package.h"
#include "simulation.h"


class Game {
public:
	/** The main Lua state for this game. */
	LuaState lua;
	
	/** The simulation core of the game. */
	Simulation simulation;
	/** The animation core of the game. */
	Animation animation;
	
	/** The Package this game uses to provide items and gameplay. */
	Package package;
	
	Game();
	
	/** Advances the game by the given amount of seconds. The dt value is considered being in the
	 base time unit of the game. You may speed up the game by tweaking this factor. Make sure dt
	 doesn't exceed a certain value so the simulation is able to run at a minimal frequency without
	 freezing up because it tries to keep up. */
	void advance(double dt);
};
