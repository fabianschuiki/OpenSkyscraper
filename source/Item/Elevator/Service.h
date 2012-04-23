#pragma once
#include "Elevator.h"

namespace OT {
	namespace Item {
		namespace Elevator {
			class Service : public Elevator {
			public:
				Service(Game * game, AbstractPrototype * prototype) : Elevator(game, prototype) {}
				OT_ITEM_PROTOTYPE(Service) {
					p->id    = "elevator-service";
					p->name  = "Service Elevator";
					p->price = 80000;
					p->size  = int2(4,1);
					p->icon  = 5;
				}
				
				virtual void init()
				{
					shaftBitmap = "simtower/elevator/narrow";
					carBitmap   = "simtower/elevator/service";
					Elevator::init();
				}
			};
		}
	}
}