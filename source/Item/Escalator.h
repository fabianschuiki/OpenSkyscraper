#pragma once
#include "Stairlike.h"

namespace OT {
	namespace Item {
		
		class Escalator : public Stairlike
		{
		public:
			Escalator(Game * game, OT::Item::AbstractPrototype * prototype) : Stairlike(game, prototype) {}
			OT_ITEM_PROTOTYPE(Escalator) {
				p->id    = "escalator";
				p->name  = "Escalator";
				p->price = 20000;
				p->size  = int2(8,2);
				p->icon  = 3;
			}
			
			virtual void init();
		};
	}
}
