#pragma once
#include "../math/vector2D.h"

class Game;


class Package {
public:
	/** The game this package is associated with. */
	Game * const game;
	
	/** The package ID. */
	std::string id;
	
	/** The size of one grid cell. All item dimensions are relative to this value. */
	int2 cell;
	
	/** Returns a string representation of the package. */
	std::string description() const;
	
	Package(Game * game);
};
