#pragma once
#include <list>
#include <map>
#include "MapNode.h"


namespace OT {
	class FloorNode;
	namespace Item { 
		class Item;
		namespace Elevator { class Elevator; }
	}

	class GameMap {
	public:
		GameMap();
		~GameMap();

		MapNode* addNode(const MapNode::Point &p, Item::Item *item);
		void removeNode(const MapNode::Point &p, Item::Item *item);
		MapNode* findNode(const MapNode::Point &p, Item::Item *item);
		void handleElevatorResize(Item::Elevator::Elevator *draggingElevator, bool draggingElevatorLower, int &draggingElevatorStart);

	private:
		std::map<MapNode::Point, MapNode *> gameMap;
		std::map<int, std::list<MapNode*> > mapNodesByFloor;
		std::map<int, FloorNode *> floorNodes;

		void clear();
	};
}