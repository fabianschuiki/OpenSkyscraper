#pragma once
#include <list>


namespace OT {
	class FloorNode;
	namespace Item { class Item; }

	class MapNode {
	public:
		struct Point {
			int x, y;

			Point() : x(0), y(0) {};
			Point(int X, int Y) : x(X), y(Y) {};

			bool operator==(const Point &rhs) const {
				return(x == rhs.x && y == rhs.y);
			}
			bool operator<(const Point &rhs) const {
				return(x < rhs.x) ||((x == rhs.x) && y < rhs.y);
			}
		};

		Point position;
		MapNode *neighbours[4];
		enum Direction {
			UP    = 0,
			DOWN  = 1, 
			LEFT  = 2, 
			RIGHT = 3
		};
		
		bool hasElevator;
		bool hasServiceElevator;
		Item::Item *transportItems[2];
		
		FloorNode *floorNode; // NULL if MapNode is actually a FloorNode, otherwise is points to the corresponding FloorNode
		std::list<MapNode *> *nodesOnFloor;

		explicit MapNode(FloorNode *floor);

	private:
		void init();
	};
}
