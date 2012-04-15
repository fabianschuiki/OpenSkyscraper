#pragma once
#include <vector>
#include <set>

namespace OT {
	namespace Item { class Item; }
	
	class Route
	{
	public:
		struct Node {
			Item::Item * item;
			int floor;
		};
		
		std::vector<Node> nodes;
		void clear();
		bool empty();
		void add(Item::Item * item);
		void add(Item::Item * item, int floor);
		bool usesItem(Item::Item * item);
		
	private:
		std::set<Item::Item *> usedItems;
	};
}