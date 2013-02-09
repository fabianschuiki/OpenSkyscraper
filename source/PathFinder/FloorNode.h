#pragma once
#include "MapNode.h"

namespace OT {
	class FloorNode : public MapNode {
	public:
		FloorNode(std::list<MapNode *> *nodeList) : MapNode(NULL) { nodesOnFloor = nodeList; }
	};
}
