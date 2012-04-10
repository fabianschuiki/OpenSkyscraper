#pragma once
#include "Item.h"

namespace OT {
	namespace Item {
		class FastFood : public Item
		{
		public:
			static AbstractPrototype * makePrototype() {
				AbstractPrototype * p = new Prototype<FastFood>;
				p->name  = "Fast Food";
				p->price = 100000;
				p->size  = int2(16,1);
				p->icon  = int2(3,1);
				return p;
			}
			OT_ITEM_CONSTRUCTOR(FastFood) {}
		};
	}
}
