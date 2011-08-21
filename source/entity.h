#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Space;
class SpacePartition;


class Entity : public sf::Sprite {
	friend class Space;
	
protected:
	std::set<SpacePartition *> partitions;
	bool visible;
	
public:
	double z;
};
