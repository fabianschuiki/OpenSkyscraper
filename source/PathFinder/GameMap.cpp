#include <cassert>
#include "../Item/Item.h"
#include "../Item/Elevator/Elevator.h"
#include "FloorNode.h"
#include "GameMap.h"
using namespace OT;


GameMap::GameMap() {}

GameMap::~GameMap() {
	clear();
}

void GameMap::clear() {
	for(std::map<MapNode::Point, MapNode *>::iterator i = gameMap.begin(); i != gameMap.end(); i++)
		delete i->second;
	gameMap.clear();
	mapNodesByFloor.clear();
}

MapNode* GameMap::addNode(const MapNode::Point &p, Item::Item *item) {
	if(!item) return NULL;
	//LOG(DEBUG, "Adding map node at %dx%d", p.x, p.y);

	// Create corresponding FloorNode if not available
	FloorNode *f;
	if(floorNodes.count(p.y) == 0) {
		f = new FloorNode(&mapNodesByFloor[p.y]);
		floorNodes[p.y] = f;
		f->position.x = INT_MIN;
		f->position.y = p.y;
	} else f = floorNodes[p.y];

	if(!item->canHaulPeople()) return f; // Building item, do not add into transport map

	MapNode *n;
	if(gameMap.count(p) == 0) {
		n = new MapNode(f);
		gameMap[p] = n;
		n->position = p;

		// Just insert node if list is empty
		if(mapNodesByFloor[p.y].empty()) mapNodesByFloor[p.y].push_back(n);
		else {
			// Look for nearby nodes on same floor to insert
			MapNode *left = NULL;
			MapNode *right = NULL;
			for(std::list<MapNode *>::iterator i = mapNodesByFloor[p.y].begin(); i != mapNodesByFloor[p.y].end(); i++) {
				MapNode *node = *i;
				if(node->position.x <= p.x) left = node;
				else {
					right = node;
					mapNodesByFloor[p.y].insert(i, n);
					break;
				}
			}

			if(left) {
				if(!right) mapNodesByFloor[p.y].push_back(n); // Insert node as last node in list
				n->neighbours[MapNode::LEFT] = left;
				left->neighbours[MapNode::RIGHT] = n;
			} 
		
			if(right) {
				n->neighbours[MapNode::RIGHT] = right;
				right->neighbours[MapNode::LEFT] = n;
			}
		}
	} else {
		n = gameMap[p];
	}

	if(item->isStairlike() && p.y == item->position.y) {
		// If stairlike, automatically create pair node above and link accordingly
		assert(n->neighbours[MapNode::UP] == NULL);
		assert(n->transportItems[MapNode::UP] == NULL);
		MapNode *n_upper = addNode(MapNode::Point(p.x, p.y + 1), item);

		n->neighbours[MapNode::UP] = n_upper;
		n->transportItems[MapNode::UP] = item;

		assert(n_upper->neighbours[MapNode::DOWN] == NULL);
		assert(n_upper->transportItems[MapNode::DOWN] == NULL);
		n_upper->neighbours[MapNode::DOWN] = n;
		n_upper->transportItems[MapNode::DOWN] = item;
	} else if(item->isElevator()) {
		n->hasElevator = true;
		// Link to upper/lower floor node
		Item::Elevator::Elevator *e =(Item::Elevator::Elevator *) item;
		for(int i = p.y + 1; i < item->position.y + item->size.y; i++) {
			if(e->connectsFloor(i)) {
				MapNode::Point ep(p.x, i);
				if(gameMap.count(ep) == 0) addNode(ep, item);
				else {
					MapNode *upper = gameMap[ep];
					n->neighbours[MapNode::UP] = upper;
					n->transportItems[MapNode::UP] = item;

					if(upper->neighbours[MapNode::DOWN]) {
						n->neighbours[MapNode::DOWN] = upper->neighbours[MapNode::DOWN];
						n->transportItems[MapNode::DOWN] = item;
					}

					upper->neighbours[MapNode::DOWN] = n;
					upper->transportItems[MapNode::DOWN] = item;
				}
				break;
			}
		}

		for(int i = p.y - 1; i >= item->position.y; i--) {
			if(e->connectsFloor(i)) {
				assert(gameMap.count(MapNode::Point(p.x, i)) != 0);
				MapNode *lower = gameMap[MapNode::Point(p.x, i)];
				n->neighbours[MapNode::DOWN] = lower;
				n->transportItems[MapNode::DOWN] = item;

				if(lower->neighbours[MapNode::UP]) {
					n->neighbours[MapNode::UP] = lower->neighbours[MapNode::UP];
					n->transportItems[MapNode::UP] = item;
				}

				lower->neighbours[MapNode::UP] = n;
				lower->transportItems[MapNode::UP] = item;
				break;
			}
		}
	}

	return n;
}

void GameMap::removeNode(const MapNode::Point &p, Item::Item *item) {
	if(!item) return;
	if(!item->canHaulPeople() || gameMap.count(p) == 0) return;
	MapNode *n = gameMap[p];
	//LOG(DEBUG, "Removing node %s at %dx%d", item->desc().c_str(), p.x, p.y);

	// Update all neighbour links
	if(item->isStairlike() && p.y == item->position.y) {
		n->neighbours[MapNode::UP]->neighbours[MapNode::DOWN] = NULL;
		n->neighbours[MapNode::UP]->transportItems[MapNode::DOWN] = NULL;

		n->neighbours[MapNode::UP] = NULL;
		n->transportItems[MapNode::UP] = NULL;
		
		removeNode(MapNode::Point(p.x, p.y + 1), item);
	} else if(item->isElevator()) {
		assert(n->hasElevator == true);
		n->hasElevator = false;

		// Link upper & lower floor node to skip this node
		if(n->neighbours[MapNode::UP]) {
			n->neighbours[MapNode::UP]->neighbours[MapNode::DOWN] = n->neighbours[MapNode::DOWN];
			if(!n->neighbours[MapNode::DOWN]) {
				n->neighbours[MapNode::UP]->transportItems[MapNode::DOWN] = NULL;
			}
		}

		if(n->neighbours[MapNode::DOWN]) {
			n->neighbours[MapNode::DOWN]->neighbours[MapNode::UP] = n->neighbours[MapNode::UP];
			if(!n->neighbours[MapNode::UP]) {
				n->neighbours[MapNode::DOWN]->transportItems[MapNode::UP] = NULL;
			}
		}

		n->neighbours[MapNode::UP] = NULL;
		n->transportItems[MapNode::UP] = NULL;
		n->neighbours[MapNode::DOWN] = NULL;
		n->transportItems[MapNode::DOWN] = NULL;
	}

	// Delete and erase only if no other overlapping item
	if( !n->hasElevator && 
		!n->transportItems[MapNode::UP] && !n->transportItems[MapNode::DOWN]) {
		//LOG(DEBUG, "Deleting node at %dx%d", p.x, p.y);
		if(n->neighbours[MapNode::LEFT]) {
			n->neighbours[MapNode::LEFT]->neighbours[MapNode::RIGHT] = n->neighbours[MapNode::RIGHT];
		}

		if(n->neighbours[MapNode::RIGHT]) {
			n->neighbours[MapNode::RIGHT]->neighbours[MapNode::LEFT] = n->neighbours[MapNode::LEFT];
		}

		gameMap.erase(p);
		for(std::list<MapNode *>::iterator i = mapNodesByFloor[p.y].begin(); i != mapNodesByFloor[p.y].end(); i++) {
			if(*i == n) {
				mapNodesByFloor[p.y].erase(i);
				break;
			}
		}
		delete n;
	}
}

MapNode* GameMap::findNode(const MapNode::Point &p, Item::Item *item) {
	if(!item) return NULL;
	if(!item->canHaulPeople()) {
		if(floorNodes.count(p.y) == 0) return NULL;
		else return floorNodes[p.y];
	}

	MapNode *n = NULL;
	if(gameMap.count(p) == 0) return NULL;
	else n = gameMap[p];

	if(item->isElevator() && (!n->hasElevator || (n->transportItems[MapNode::UP] != item && n->transportItems[MapNode::DOWN] != item)))
		return NULL;
	else if(item->isStairlike() && n->transportItems[MapNode::UP] != item)
		return NULL;
	else
		return n;
}

void GameMap::handleElevatorResize(Item::Elevator::Elevator *draggingElevator, bool draggingElevatorLower, int &draggingElevatorStart) {
	MapNode::Point p(draggingElevator->position.x + draggingElevator->size.x/2, 0);
	if (draggingElevatorLower) {
		if (draggingElevatorStart <= draggingElevator->position.y) {
			// Elevator bottom shifted up
			int previousLowestAccessibleFloor = draggingElevatorStart;
			while (previousLowestAccessibleFloor < draggingElevator->position.y) {
				p.y = previousLowestAccessibleFloor;
				if (findNode(p, draggingElevator)) removeNode(p, draggingElevator);
				previousLowestAccessibleFloor++;
			}
		} else {
			// Elevator bottom shifted down
			p.y = draggingElevator->position.y;
			while (p.y < draggingElevatorStart) {
				if (draggingElevator->connectsFloor(p.y)) {
					addNode(p, draggingElevator);
					break;
				}
				p.y++;
			}
		}
		draggingElevatorStart = draggingElevator->position.y;
	} else {
		if (draggingElevatorStart >= draggingElevator->position.y + draggingElevator->size.y - 1) {
			// Elevator top shifted down
			int previousHighestAccessibleFloor = draggingElevatorStart;
			while (previousHighestAccessibleFloor >= draggingElevator->position.y + draggingElevator->size.y) {
				p.y = previousHighestAccessibleFloor;
				if (findNode(p, draggingElevator)) removeNode(p, draggingElevator);
				previousHighestAccessibleFloor--;
			}
		} else {
			// Elevator top shifted up
			p.y = draggingElevatorStart + 1;
			while (p.y < draggingElevator->position.y + draggingElevator->size.y) {
				if (draggingElevator->connectsFloor(p.y)) {
					addNode(p, draggingElevator);
					break;
				}
				p.y++;
			}
		}
		draggingElevatorStart = draggingElevator->position.y + draggingElevator->size.y - 1;
	}
}
