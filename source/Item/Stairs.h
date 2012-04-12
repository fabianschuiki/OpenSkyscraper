#pragma once
#include "Stairlike.h"

namespace OT {
	namespace Item {
		
		class Stairs : public Stairlike
		{
		public:
			Stairs(Game * game, OT::Item::AbstractPrototype * prototype) : Stairlike(game, prototype) {}
			OT_ITEM_PROTOTYPE(Stairs) {
				p->id    = "stairs";
				p->name  = "Stairs";
				p->price = 5000;
				p->size  = int2(8,2);
				p->icon  = 2;
			}
			
			virtual void init();
		};
	}
}
