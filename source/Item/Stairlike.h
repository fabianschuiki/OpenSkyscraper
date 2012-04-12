#pragma once
#include "Item.h"

namespace OT {
	namespace Item {
		
		class Stairlike : public Item
		{
		public:
			Stairlike(Game * game, OT::Item::AbstractPrototype * prototype) : Item(game, prototype) {}
			
			virtual void init();
			
			Sprite sprite;
			double animation;
			int frame;
			int frameCount;
			
			void updateSprite();
			virtual void advance(double dt);
		};
	}
}
