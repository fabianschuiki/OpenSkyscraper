#include <cmath>
#include "../Item/Item.h"
#include "FloorNode.h"
#include "MapSearchNode.h"
using namespace OT;


MapSearchNode::MapSearchNode() : mapNode(NULL), parent_item(NULL), 
								 numStairs(0), numEscalators(0), numElevators(0), 
								 g(0), h(0), 
								 start_point(INT_MIN, INT_MIN), end_point(INT_MIN, INT_MIN),
								 serviceRoute(false) {}

MapSearchNode::MapSearchNode(const OT::MapNode *mp) : mapNode(mp), parent_item(NULL), 
													  numStairs(0), numEscalators(0), numElevators(0), 
													  g(0), h(0), 
													  start_point(INT_MIN, INT_MIN), end_point(INT_MIN, INT_MIN),
													  serviceRoute(false) {}

float MapSearchNode::GetCost(MapSearchNode &successor) {
	Item::Item *i = getItemOnRoute(&successor); // Discover item that lies on this node, given the next step
	successor.parent_item = i;
	successor.numStairs = numStairs;
	successor.numEscalators = numEscalators;
	successor.numElevators = numElevators;
	successor.g = g;
	
	int traverse_cost = 0;
	if(!i) {
		// NULL item. Next step is traversal on current floor.
		if(mapNode->position.y == 0 && successor.mapNode->position.y == 0)
			traverse_cost = 0; // Movement in main lobby is free(?)
		else if(!mapNode->floorNode && start_point.x > INT_MIN) // This is a floor start node, use start_point to compute cost instead
			traverse_cost = std::abs(successor.mapNode->position.x - start_point.x)*WALKING_COST;
		else if(!successor.mapNode->floorNode && mapNode->position.y == end_point.y) {
			// Successor is a floor end node, use end_point to compute cost instead
			int end_walking_dist = std::abs(end_point.x - mapNode->position.x);

			// Check if final transport node is too far away from destination, and adjust cost accordingly
			if(end_walking_dist > MAX_WALKING_DIST)
				traverse_cost = (int)(MAX_WALKING_DIST*WALKING_COST + (float)end_walking_dist/MAX_WALKING_DIST*INHIBITORY_COST);
			else
				traverse_cost = end_walking_dist*WALKING_COST;
		} else
			traverse_cost = std::abs(successor.mapNode->position.x - mapNode->position.x)*WALKING_COST;
	} else if(i->isStairlike()) {
		if(i->prototype->icon == 2) {
			traverse_cost += STAIRS_COST;
			successor.numStairs++;
		} else {
			traverse_cost += ESCALATOR_COST;
			successor.numEscalators++;
		}
		
		traverse_cost += FLOOR_COST;
	} else if(i->isElevator()) {
		if(i != parent_item) {
			// NOTE: In future when hotel rooms are implemented, 
			// there will need to be another check here to include Service elevator costs.
			if(i->prototype->icon == 4)
				traverse_cost += ELEVATOR_COST;
			else
				traverse_cost += EXPRESS_COST;
			successor.numElevators++;
		}
		
		traverse_cost += std::abs(successor.mapNode->position.y - mapNode->position.y)*FLOOR_COST;
	} else traverse_cost = INHIBITORY_COST;	// ERROR: Should not end up here. INHIBITORY_COST should deter further expansion from this node.

	successor.g += (float)traverse_cost;
	return (float)traverse_cost;
}

float MapSearchNode::GoalDistanceEstimate(MapSearchNode &nodeGoal) {
	if(!nodeGoal.mapNode->floorNode) {
		if(!mapNode->floorNode)
			h = (float)(std::abs(end_point.y - mapNode->position.y)*FLOOR_COST);
		else
			h = (float)(std::abs(end_point.x - mapNode->position.x)*WALKING_COST + std::abs(end_point.y - mapNode->position.y)*FLOOR_COST);
	} else {
		if(!mapNode->floorNode)
			h = (float)(std::abs(nodeGoal.mapNode->position.x - start_point.x)*WALKING_COST + std::abs(nodeGoal.mapNode->position.y - start_point.y)*FLOOR_COST);
		else
			h = (float)(std::abs(nodeGoal.mapNode->position.x - mapNode->position.x)*WALKING_COST + std::abs(nodeGoal.mapNode->position.y - mapNode->position.y)*FLOOR_COST);
	}

	return h;
}

bool MapSearchNode::GetSuccessors(AStarSearch< MapSearchNode > *astarsearch, MapSearchNode *parent_node) {
	MapSearchNode n;
	MapNode *left = NULL;
	MapNode *right = NULL;
	bool ret;

	if(!parent_node && !mapNode->floorNode) {
		// Start node which is also a floor node.
		// Find and add nearest transport nodes to the left/right (based on start_point, which should be populated)
		std::list<MapNode *> *nodesOnFloor = mapNode->nodesOnFloor;
		assert(nodesOnFloor != NULL);
		for(std::list<MapNode *>::const_iterator i = nodesOnFloor->begin(); i != nodesOnFloor->end(); i++) {
			MapNode *node = *i;
			if(node->position.x <= start_point.x && canTransfer(this, node, MapNode::LEFT)) left = node;
			else if(canTransfer(this, node, MapNode::RIGHT)) {
				right = node;
				break;
			}
		}

		if(left) {
			createNode(n, left);
			ret = astarsearch->AddSuccessor(n);
			if(!ret) return false;
		}
		
		if(right) {
			createNode(n, right);
			ret = astarsearch->AddSuccessor(n);
			if(!ret) return false;
		}

		return true;
	}

	if(mapNode->position.y == end_point.y && !astarsearch->GetSolutionEnd()->mapNode->floorNode) {
		// Reached the same floor as end node, and end node is a floor node.
		// Add destination floor node
		if(!mapNode->floorNode) return false; // ERROR: All transport nodes must contain pointer to their respective floor node.

		createNode(n, mapNode->floorNode);
		ret = astarsearch->AddSuccessor(n);
		if(!ret) return false;
		else	 return true;
	}

	MapNode *node = mapNode->neighbours[MapNode::LEFT];
	while(node && !left) {
		if (!canTransfer(this, node, MapNode::LEFT)) {
			node = node->neighbours[MapNode::LEFT];
			continue;
		}
		
		left = node;
	}

	node = mapNode->neighbours[MapNode::RIGHT];
	while(node && !right) {
		if (!canTransfer(this, node, MapNode::RIGHT)) {
			node = node->neighbours[MapNode::RIGHT];
			continue;
		}

		right = node;
	}

	if(left) {
		createNode(n, left);
		ret = astarsearch->AddSuccessor(n);
		if(!ret) return false;
	}

	if(right) {
		createNode(n, right);
		ret = astarsearch->AddSuccessor(n);
		if(!ret) return false;
	}

	if(mapNode->neighbours[MapNode::UP]) {
		node = mapNode->neighbours[MapNode::UP];
		if(mapNode->hasElevator) {
			while(node) {
				createNode(n, node);
				ret = astarsearch->AddSuccessor(n);
				if(!ret) return false;
				node = node->neighbours[MapNode::UP];
			}
		} else if(canTransfer(this, node, MapNode::UP)) {
			createNode(n, node);
			ret = astarsearch->AddSuccessor(n);
			if(!ret) return false;
		}
	}

	if(mapNode->neighbours[MapNode::DOWN]) {
		node = mapNode->neighbours[MapNode::DOWN];
		if(mapNode->hasElevator) {
			while(node) {
				createNode(n, node);
				ret = astarsearch->AddSuccessor(n);
				if(!ret) return false;
				node = node->neighbours[MapNode::DOWN];
			}
		} else if(canTransfer(this, node, MapNode::DOWN)) {
			createNode(n, node);
			ret = astarsearch->AddSuccessor(n);
			if(!ret) return false;
		}
	}

	return true;
}

bool MapSearchNode::IsGoal(MapSearchNode &nodeGoal) {
	if(mapNode == nodeGoal.mapNode) {
		nodeGoal.g = g;
		nodeGoal.h = h;
		return true;
	} else return false;
}

bool MapSearchNode::IsSameState(MapSearchNode &rhs) {
	return (mapNode == rhs.mapNode);
}

OT::Item::Item * MapSearchNode::getItemOnRoute(const MapSearchNode *successor) {
	// NOTE: Calling this on the start and end nodes will not give correct results.
	// Always use known start and destination items instead.

	const OT::MapNode *s_mapnode;

	if(!successor || !successor->mapNode) return NULL;
	else s_mapnode = successor->mapNode;

	if(mapNode->neighbours[MapNode::UP] == s_mapnode)
		 return mapNode->transportItems[MapNode::UP];
	else if(mapNode->neighbours[MapNode::DOWN] == s_mapnode)
		 return mapNode->transportItems[MapNode::DOWN];
	else if(mapNode->transportItems[MapNode::UP] == s_mapnode->transportItems[MapNode::DOWN])
		return mapNode->transportItems[MapNode::UP];
	else if(mapNode->transportItems[MapNode::DOWN] == s_mapnode->transportItems[MapNode::UP])
		return mapNode->transportItems[MapNode::DOWN];
	else
		return NULL;
}

bool MapSearchNode::canTransfer(const MapSearchNode *start, const OT::MapNode *dest, const OT::MapNode::Direction dir) const {
	/* 
		Returns true if transfer to destination transport has not exceed transfer limits.
		Returns false otherwise.
		Maximum transfer limits are:
			2 elevators (no more than 1 elevator if journey has already used stairs or escalators)
			3 stairs
			6 escalators
			1 stair, 2 escalators OR 2 stairs, 1 escalator
	*/

	if((dir == MapNode::UP || dir == MapNode::DOWN)) {
		// Once in an elevator node, travel is allowed for as many floors available in the elevator shaft
		if(start->mapNode->hasElevator) return true;

		// Hence we only check limits for stairlike travel
		if(start->mapNode->transportItems[dir]->prototype->icon == 2) {
			if(start->numStairs > 2 - start->numEscalators) return false;
		} else {
			if(start->numEscalators > 5) return false;
			if(start->numStairs > 0 && start->numEscalators > 2 - start->numStairs) return false;
		}
	} else {
		if(dest->hasElevator) {
			if((!start->serviceRoute && dest->hasServiceElevator) || (start->serviceRoute && !dest->hasServiceElevator)) return false;
			if(start->numElevators > 1) return false;
			if(start->numElevators == 1 && (start->mapNode->position.y % 15 != 0 || start->numStairs > 0 || start->numEscalators > 0)) return false;
		}
		// Transit to other stairlike nodes (moving left/right) has no limits.
		// Limit only applies when actually using (moving up/down) the stairlike.
	}

	return true;
}

void MapSearchNode::createNode(MapSearchNode &n, const OT::MapNode *node) {
	n = MapSearchNode(node);
	n.end_point = end_point;
	n.serviceRoute = serviceRoute;
}
