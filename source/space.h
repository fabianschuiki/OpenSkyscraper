#pragma once
#include <map>
#include <set>
#include <vector>
#include <SFML/Graphics/Rect.hpp>

class Sprite;
class SpacePartition;
class SpaceSlice;
namespace sf { class RenderTarget; }


class Space {
public:
	~Space();
	
	void addSprite(Sprite * s);
	void removeSprite(Sprite * s);
	
	void setVisibleRect(const sf::FloatRect & r);
	const std::vector<Sprite *> & getSortedVisibleSprites();
	
	void draw(sf::RenderTarget & context);
	
private:
	std::set<Sprite *> sprites;
	std::set<Sprite *> visibleSprites;
	std::vector<Sprite *> sortedVisibleSprites;
	
	void insertVisibleSprite(Sprite * s);
	
	std::set<SpacePartition *> partitions;
	std::set<SpacePartition *> emptyPartitions;
	std::set<SpacePartition *> visiblePartitions;
	std::map<int, std::map<int, SpacePartition *> > partitionGrid;
	
	sf::FloatRect visibleRect;
	void toPartitionCoordinates(int & minx, int & miny, int & maxx, int & maxy);
	
	void remapSprite(Sprite * s);
	void cleanEmptyPartitions();
};
