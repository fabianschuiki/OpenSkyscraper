#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream>
#include "space.h"
#include "spacepartition.h"
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


void Space::setVisibleRect(const sf::FloatRect & r)
{
	visibleRect = r;
	
	//Clear the currently visible partitions.
	for (std::set<SpacePartition *>::iterator p = visiblePartitions.begin();
		 p != visiblePartitions.end(); p++)
		(*p)->visible = false;
	visiblePartitions.clear();
	
	//Find new visible partitions.
	int minx = r.Left, miny = r.Top, maxx = r.Right, maxy = r.Bottom;
	toPartitionCoordinates(minx, miny, maxx, maxy);
	for (int x = minx; x <= maxx; x++) {
		for (int y = miny; y <= maxy; y++) {
			if (partitionGrid.count(x) && partitionGrid[x].count(y)) {
				SpacePartition * p = partitionGrid[x][y];
				visiblePartitions.insert(p);
				p->visible = true;
			}
		}
	}
	
	//Clear the list of visible sprites.
	for (std::set<Sprite *>::iterator s = visibleSprites.begin(); s != visibleSprites.end(); s++)
		(*s)->visible = false;
	visibleSprites.clear();
	sortedVisibleSprites.clear();
	
	//Build the sorted list of visible sprites.
	for (std::set<SpacePartition *>::iterator p = visiblePartitions.begin();
		 p != visiblePartitions.end(); p++) {
		for (std::set<Sprite *>::iterator s = (*p)->sprites.begin();
			 s != (*p)->sprites.end(); s++) {
			insertVisibleSprite(*s);
		}
	}
}

const std::vector<Sprite *> & Space::getSortedVisibleSprites()
{
	return sortedVisibleSprites;
}


void Space::draw(sf::RenderTarget & context)
{
	//Draw all partitions for debugging purposes.
	for (std::set<SpacePartition *>::iterator p = partitions.begin(); p != partitions.end(); p++)
		(*p)->draw(context);
}


void Space::insertVisibleSprite(Sprite * s)
{
	if (visibleSprites.count(s)) return;
	s->visible = true;
	visibleSprites.insert(s);
	
	//Find the right place to insert this sprite through a binary search.
	int f = 0, l = sortedVisibleSprites.size(), i = 0;
	while (f < l) {
		i = (f + l) / 2;
		std::cout << "   trying " << i << "\n";
		float z = sortedVisibleSprites[i]->z;
		if (z < s->z) f = i = i + 1;
		if (z > s->z) l = i;
		if (z == s->z) f = l = i;
	};
	
	//Insert the sprite there.
	std::cout << "inserting " << s << " at " << i << "\n";
	sortedVisibleSprites.insert(sortedVisibleSprites.begin() + i, s);
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
