#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream>
#include "space.h"
#include "spacepartition.h"
#include "spaceslice.h"
#include "sprite.h"


Space::~Space()
{
	for (std::set<SpacePartition *>::iterator p = partitions.begin(); p != partitions.end(); p++)
		delete *p;
	partitions.clear();
	partitionGrid.clear();
}


void Space::addSprite(Sprite * s)
{
	sprites.insert(s);
	remapSprite(s);
}

void Space::removeSprite(Sprite * s)
{
	sprites.erase(s);
}


SpaceSlice * Space::makeSlice(int x0, int y0, int x1, int y1)
{
	SpaceSlice * s = new SpaceSlice(x0, y0, x1, y1);
	toPartitionCoordinates(x0, y0, x1, y1);
	for (int x = x0; x <= x1; x++)
		for (int y = y0; y <= y1; y++)
			if (partitionGrid.count(x) && partitionGrid[x].count(y))
				s->partitions.insert(partitionGrid[x][y]);
	return s;
}


void Space::draw(sf::RenderTarget & context)
{
	//Draw all partitions for debugging purposes.
	for (std::set<SpacePartition *>::iterator p = partitions.begin(); p != partitions.end(); p++)
		(*p)->draw(context);
}


void Space::toPartitionCoordinates(int & minx, int & miny, int & maxx, int & maxy)
{
	minx = round((double)minx / SpacePartition::w);
	miny = round((double)miny / SpacePartition::h);
	maxx = round((double)maxx / SpacePartition::w);
	maxy = round((double)maxy / SpacePartition::h);
}


void Space::remapSprite(Sprite * s)
{
	//Find the outer dimensions of the sprite.
	const sf::Vector2f & vp = s->GetPosition();
	const sf::Vector2f & vs = s->GetSize();
	int minx = vp.x;
	int miny = vp.y;
	int maxx = minx + vs.x;
	int maxy = miny + vs.y;
	std::cout << "(" << minx << " -> " << maxx << ") x (" << miny << " -> " << maxy << ") => ";
	
	//Fit the coordinates to space partition segments.
	toPartitionCoordinates(minx, miny, maxx, maxy);
	std::cout << "(" << minx << " -> " << maxx << ") x (" << miny << " -> " << maxy << ")\n";
	
	//Unlink the sprite from its existing partitions.
	for (std::set<SpacePartition *>::iterator p = s->partitions.begin();
		 p != s->partitions.end(); p++) {
		(*p)->sprites.erase(s);
		if ((*p)->sprites.empty())
			emptyPartitions.insert(*p);
	}
	s->partitions.clear();
	
	//Map the sprite to these coordinates.
	for (int x = minx; x <= maxx; x++) {
		for (int y = miny; y <= maxy; y++) {
			
			//Get the partition at this location, creating one if there's none.
			SpacePartition * p = partitionGrid[x][y];
			if (!p) {
				std::cout << "created partition at " << x << " x " << y << "\n";
				p = new SpacePartition(x, y);
				partitionGrid[x][y] = p;
				partitions.insert(p);
			}
			
			//Link the partition and sprite.
			p->sprites.insert(s);
			s->partitions.insert(p);
		}
	}
}

void Space::cleanEmptyPartitions()
{
	for (std::set<SpacePartition *>::iterator p = emptyPartitions.begin();
		 p != emptyPartitions.end(); p++) {
		if ((*p)->sprites.empty()) {
			partitionGrid[(*p)->x].erase((*p)->y);
			partitions.erase(*p);
			delete *p;
		}
	}
	emptyPartitions.clear();
}
