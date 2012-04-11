#pragma once
#include "Item.h"

namespace OT {
	namespace Item {
		class FastFood : public Item
		{
		public:
			OT_ITEM_CONSTRUCTOR(FastFood);
			OT_ITEM_PROTOTYPE(FastFood) {
				p->id    = "fastfood";
				p->name  = "Fast Food";
				p->price = 100000;
				p->size  = int2(16,1);
				p->icon  = 11;
			}
			
			virtual void init();
			
			virtual void encodeXML(tinyxml2::XMLPrinter & xml);
			virtual void decodeXML(tinyxml2::XMLElement & xml);
			
			int variant;
			
			sf::Sprite sprite;
			void updateSprite();
		};
	}
}
