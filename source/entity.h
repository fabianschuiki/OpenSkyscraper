#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Item;
class Space;
class SpacePartition;


class Entity : public sf::Sprite {
	friend class Item;
	friend class Space;
	
public:
	double z;
	
	Entity();
	
	Item * getOwner() const;
	
protected:
	std::set<SpacePartition *> partitions;
	bool visible;
	
	/** The item this sprite is currently associated with. */
	Item * owner;
};
