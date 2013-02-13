#pragma once
#include "../Sprite.h"
#include "Item.h"

namespace OT {
	namespace Item {
		class Metro : public Item
		{
		public:
			OT_ITEM_CONSTRUCTOR(Metro);
			OT_ITEM_PROTOTYPE(Metro) {
				p->id    = "metro";
				p->name  = "Metro Station";
				p->price = 1000000;
				p->size  = int2(30,3);
				p->icon  = 19;
			}
			virtual ~Metro();
			
			virtual void init();
			
			virtual void encodeXML(tinyxml2::XMLPrinter & xml);
			virtual void decodeXML(tinyxml2::XMLElement & xml);
			
			bool open;
			bool trainPresent;
			
			Sprite station;
			Sprite platform;
			bool spriteNeedsUpdate;
			void updateSprite();
			
			virtual void advance(double dt);
		};
	}
}
