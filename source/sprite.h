#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <vector>

class Space;
class SpacePartition;


class Sprite : public sf::Sprite {
	friend class Space;
	
protected:
	std::set<SpacePartition *> partitions;
	bool visible;
	
public:
	double z;
};
