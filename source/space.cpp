#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream>
#include "space.h"
#include "spacepartition.h"
#include "entity.h"


Space::~Space()
{
	for (std::set<SpacePartition *>::iterator p = partitions.begin(); p != partitions.end(); p++)
		delete *p;
	partitions.clear();
	partitionGrid.clear();
}


void Space::addEntity(Entity * s)
{
	entities.insert(s);
	remapEntity(s);
}

void Space::removeEntity(Entity * s)
{
	entities.erase(s);
}


void Space::setVisibleRect(const rectd & r)
{
	visibleRect = r;
	
	//Clear the currently visible partitions.
	for (std::set<SpacePartition *>::iterator p = visiblePartitions.begin();
		 p != visiblePartitions.end(); p++)
		(*p)->visible = false;
	visiblePartitions.clear();
	
	//Find new visible partitions.
	int2 min = r.minXminY(), max = r.maxXmaxY();
	toPartitionCoordinates(min, max);
	for (int x = min.x; x <= max.x; x++) {
		for (int y = min.y; y <= max.y; y++) {
			if (partitionGrid.count(x) && partitionGrid[x].count(y)) {
				SpacePartition * p = partitionGrid[x][y];
				visiblePartitions.insert(p);
				p->visible = true;
			}
		}
	}
	
	//Clear the list of visible entities.
	for (std::set<Entity *>::iterator s = visibleEntitys.begin(); s != visibleEntitys.end(); s++)
		(*s)->visible = false;
	visibleEntitys.clear();
	sortedVisibleEntitys.clear();
	
	//Build the sorted list of visible entities.
	for (std::set<SpacePartition *>::iterator p = visiblePartitions.begin();
		 p != visiblePartitions.end(); p++) {
		for (std::set<Entity *>::iterator s = (*p)->entities.begin();
			 s != (*p)->entities.end(); s++) {
			insertVisibleEntity(*s);
		}
	}
}

const std::vector<Entity *> & Space::getSortedVisibleEntitys()
{
	return sortedVisibleEntitys;
}


void Space::draw(sf::RenderTarget & context)
{
	//Draw all partitions for debugging purposes.
	for (std::set<SpacePartition *>::iterator p = partitions.begin(); p != partitions.end(); p++)
		(*p)->draw(context);
}


void Space::insertVisibleEntity(Entity * s)
{
	if (visibleEntitys.count(s)) return;
	s->visible = true;
	visibleEntitys.insert(s);
	
	//Find the right place to insert this entity through a binary search.
	int f = 0, l = sortedVisibleEntitys.size(), i = 0;
	while (f < l) {
		i = (f + l) / 2;
		std::cout << "   trying " << i << "\n";
		float z = sortedVisibleEntitys[i]->z;
		if (z < s->z) f = i = i + 1;
		if (z > s->z) l = i;
		if (z == s->z) f = l = i;
	};
	
	//Insert the entity there.
	std::cout << "inserting " << s << " at " << i << "\n";
	sortedVisibleEntitys.insert(sortedVisibleEntitys.begin() + i, s);
}


void Space::toPartitionCoordinates(int2 & min, int2 & max)
{
	min.x = round((double)min.x / SpacePartition::size.x);
	min.y = round((double)min.y / SpacePartition::size.y);
	max.x = round((double)max.x / SpacePartition::size.x);
	max.y = round((double)max.y / SpacePartition::size.y);
}


void Space::remapEntity(Entity * s)
{
	//Find the outer dimensions of the entity.
	const sf::Vector2f & vp = s->GetPosition();
	const sf::Vector2f & vs = s->GetSize();
	int2 min(vp.x, vp.y);
	int2 max(min.x + vs.x, min.y + vs.y);
	std::cout << "(" << min.x << " -> " << max.x << ") x (" << min.y << " -> " << max.y << ") => ";
	
	//Fit the coordinates to space partition segments.
	toPartitionCoordinates(min, max);
	std::cout << "(" << min.x << " -> " << max.x << ") x (" << min.y << " -> " << max.y << ")\n";
	
	//Unlink the entity from its existing partitions.
	for (std::set<SpacePartition *>::iterator p = s->partitions.begin();
		 p != s->partitions.end(); p++) {
		(*p)->entities.erase(s);
		if ((*p)->entities.empty())
			emptyPartitions.insert(*p);
	}
	s->partitions.clear();
	
	//Map the entity to these coordinates.
	for (int x = min.x; x <= max.x; x++) {
		for (int y = min.y; y <= max.y; y++) {
			
			//Get the partition at this location, creating one if there's none.
			SpacePartition * p = partitionGrid[x][y];
			if (!p) {
				std::cout << "created partition at " << x << " x " << y << "\n";
				p = new SpacePartition(x, y);
				partitionGrid[x][y] = p;
				partitions.insert(p);
			}
			
			//Link the partition and entity.
			p->entities.insert(s);
			s->partitions.insert(p);
		}
	}
}

void Space::cleanEmptyPartitions()
{
	for (std::set<SpacePartition *>::iterator p = emptyPartitions.begin();
		 p != emptyPartitions.end(); p++) {
		if ((*p)->entities.empty()) {
			partitionGrid[(*p)->pos.x].erase((*p)->pos.y);
			partitions.erase(*p);
			delete *p;
		}
	}
	emptyPartitions.clear();
}
