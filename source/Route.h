#pragma once
#include <cstdlib>
#include <vector>
#include <set>

namespace OT {
	namespace Item { class Item; }
	
	class Route
	{
	public:
		struct Node {
			Item::Item * item;
			int toFloor;
			Node() {
				item = NULL;
				toFloor = 0;
			}
		};
		
		Route();
		
		std::vector<Node> nodes;
		
		void clear();
		bool empty() const;
		
		void add(Item::Item * item);
		void add(Item::Item * item, int floor);
		
		bool usesItem(Item::Item * item) const;
		int numStairs;
		int numEscalators;
		int numElevators;
		
		int score() const;
		void updateScore();
		
	private:
		std::set<Item::Item *> usedItems;
		int cached_score;
	};
}