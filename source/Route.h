#pragma once
#include <cstddef>
#include <vector>

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
		
		int numStairs;
		int numEscalators;
		int numElevators;
		
		int score() const;
		void updateScore(int score);
		
	private:
		int cached_score;
	};
}