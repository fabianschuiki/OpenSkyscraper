#pragma once
#include <set>

class Space;
class Sprite;
namespace sf { class RenderTarget; }


class SpacePartition {
	friend class Space;
	
protected:
	std::set<Sprite *> sprites;
	bool visible;
	
public:
	const int x;
	const int y;
	
	static const int w;
	static const int h;
	
	SpacePartition(int x, int y);
	
	void draw(sf::RenderTarget & context);
};
