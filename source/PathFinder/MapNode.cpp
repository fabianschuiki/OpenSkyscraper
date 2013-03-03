#include "../Item/Item.h"
#include "FloorNode.h"
#include "MapNode.h"
using namespace OT;


MapNode::MapNode(FloorNode *floor) : hasElevator(false), hasServiceElevator(false), floorNode(floor), nodesOnFloor(NULL) {
	init();
}

void MapNode::init() {
	neighbours[UP] = NULL;
	neighbours[DOWN] = NULL;
	neighbours[LEFT] = NULL;
	neighbours[RIGHT] = NULL;

	transportItems[UP] = NULL;
	transportItems[DOWN] = NULL;
}
