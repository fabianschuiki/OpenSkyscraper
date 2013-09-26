#pragma once
#include <stlastar.h>
#include "MapSearchNode.h"

namespace OT {
	namespace Item { class Item; }
	class MapNode;

	class PathFinder {
	public:
		PathFinder();
		~PathFinder();
		Route findRoute(const MapNode *start_mapnode, const MapNode *end_mapnode, Item::Item *start_item, Item::Item *end_item, bool serviceRoute);

	private:
		AStarSearch< MapSearchNode > astarsearch;
		unsigned int SearchState;

		void clear();
		void buildRoute(Route &r, Item::Item *start_item, Item::Item *end_item);
	};
}

