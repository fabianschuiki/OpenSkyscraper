#pragma once
#include <map>
#include <set>
#include <vector>
#include "math/rect.h"

class Entity;
class SpacePartition;
class SpaceSlice;
namespace sf { class RenderTarget; }


/**
 * Performs spatial organization of the entities by splitting them up into individual
 * SpacePartitions to speed up rendering.
 */
class Space {
public:
	~Space();
	
	/** Adds the given Entity, placing it in the appropriate SpacePartition. */
	void addEntity(Entity * s);
	/**
	 * Removes the given Entity. SpacePartition instances that aren't used anymore are marked as
	 * empty so they may get collected and deleted later on.
	 */
	void removeEntity(Entity * s);
	
	
	/**
	 * Sets the currently visible rect. This will cause the Space to recalculate which
	 * SpacePartition and Entity instances are visible.
	 */
	void setVisibleRect(const rectd & r);
	
	/**
	 * Returns all visible Entity instances sorted by their z coordinate, ready to be drawn.
	 */
	const std::vector<Entity *> & getSortedVisibleEntitys();
	
	void draw(sf::RenderTarget & context);
	
private:
	std::set<Entity *> entities;
	std::set<Entity *> visibleEntitys;
	std::vector<Entity *> sortedVisibleEntitys;
	
	void insertVisibleEntity(Entity * s);
	
	std::set<SpacePartition *> partitions;
	std::set<SpacePartition *> emptyPartitions;
	std::set<SpacePartition *> visiblePartitions;
	std::map<int, std::map<int, SpacePartition *> > partitionGrid;
	
	rectd visibleRect;
	void toPartitionCoordinates(int2 & min, int2 & max);
	
	void remapEntity(Entity * s);
	void cleanEmptyPartitions();
};
