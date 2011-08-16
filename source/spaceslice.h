#pragma once
#include <set>

class Space;
class SpacePartition;


class SpaceSlice {
	friend class Space;
	
public:
	const int x0, y0, x1, y1;
	
	SpaceSlice(int x0, int y0, int x1, int y1);
	
public:
	std::set<SpacePartition *> partitions;
};
