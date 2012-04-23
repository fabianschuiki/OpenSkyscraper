#pragma once
#include "Elevator.h"

namespace OT {
	namespace Item {
		namespace Elevator {
			class Standard : public Elevator {
			public:
				Standard(Game * game, AbstractPrototype * prototype) : Elevator(game, prototype) {}
				OT_ITEM_PROTOTYPE(Standard) {
					p->id    = "elevator-standard";
					p->name  = "Standard Elevator";
					p->price = 100000;
					p->size  = int2(4,1);
					p->icon  = 4;
				}
				
				virtual void init()
				{
					shaftBitmap = "simtower/elevator/narrow";
					carBitmap   = "simtower/elevator/standard";
					Elevator::init();
				}
			};
		}
	}
}