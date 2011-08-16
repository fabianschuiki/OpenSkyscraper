#pragma once
#include <map>
#include <set>

class Sprite;
class SpacePartition;
class SpaceSlice;
namespace sf { class RenderTarget; }


class Space {
public:
	~Space();
	
	void addSprite(Sprite * s);
	void removeSprite(Sprite * s);
	
	SpaceSlice * makeSlice(int x0, int y0, int x1, int y1);
	
	void draw(sf::RenderTarget & context);
	
private:
	std::set<Sprite *> sprites;
	
	std::set<SpacePartition *> partitions;
	std::set<SpacePartition *> emptyPartitions;
	std::map<int, std::map<int, SpacePartition *> > partitionGrid;
	
	void toPartitionCoordinates(int & minx, int & miny, int & maxx, int & maxy);
	
	void remapSprite(Sprite * s);
	void cleanEmptyPartitions();
};
