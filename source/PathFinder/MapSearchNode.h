#pragma once
#include <stlastar.h>
#include "GameMap.h"
#include "MapNode.h"


namespace OT { 
	namespace Item { class Item; }
}

class MapSearchNode {
public:
	const OT::MapNode *mapNode;
	OT::Item::Item *parent_item;
	int numStairs;
	int numEscalators;
	int numElevators;
	float g;
	float h;
	OT::MapNode::Point start_point;
	OT::MapNode::Point end_point;

	MapSearchNode();
	explicit MapSearchNode(const OT::MapNode *mp);

	float GetCost(MapSearchNode &successor);
	float GoalDistanceEstimate(MapSearchNode &nodeGoal);
	bool GetSuccessors(AStarSearch<MapSearchNode> *astarsearch, MapSearchNode *parent_node);
	bool IsGoal(MapSearchNode &nodeGoal);
	bool IsSameState(MapSearchNode &rhs);

	OT::Item::Item * getItemOnRoute(const MapSearchNode *successor);
	bool canTransfer(const MapSearchNode *start, const OT::MapNode *dest, const OT::MapNode::Direction dir) const;

private:
	static const int WALKING_COST	  = 1;
	static const int FLOOR_COST		  = 5;
	static const int ESCALATOR_COST	  = 10;
	static const int STAIRS_COST	  = 30;
	static const int ELEVATOR_COST	  = STAIRS_COST*3 + ESCALATOR_COST/2;
	static const int EXPRESS_COST	  = ELEVATOR_COST/2;
	static const int INHIBITORY_COST  = 10000;

	static const int MAX_WALKING_DIST = 80;
};
