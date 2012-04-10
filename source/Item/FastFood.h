#pragma once
#include "Item.h"

namespace OT {
	namespace Item {
		class FastFood : public Item
		{
		public:
			OT_ITEM_CONSTRUCTOR(FastFood);
			OT_ITEM_PROTOTYPE(FastFood) {
				p->name  = "Fast Food";
				p->price = 100000;
				p->size  = int2(16,1);
				p->icon  = int2(3,1);
			}
			
			virtual void init();
		};
	}
}
