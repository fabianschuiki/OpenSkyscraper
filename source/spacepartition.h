#pragma once
#include <set>
#include "math/vector2D.h"

class Space;
class Entity;
namespace sf { class RenderTarget; }


class SpacePartition {
	friend class Space;
	
protected:
	std::set<Entity *> entities;
	bool visible;
	
public:
	const int2 pos;
	
	static const int2 size;
	
	SpacePartition(int x, int y);
	SpacePartition(int2 pos);
	
	void draw(sf::RenderTarget & context);
};
