#pragma once
#include "../Sprite.h"
#include "Item.h"

namespace OT {
	namespace Item {
		class Floor : public Item
		{
		public:
			OT_ITEM_CONSTRUCTOR(Floor);
			OT_ITEM_PROTOTYPE(Floor) {
				p->id    = "floor";
				p->name  = "Floor";
				p->price = 500;
				p->size  = int2(1,1);
				p->icon  = 1;
			}
			virtual ~Floor();
			
			virtual void init();
			
			virtual void encodeXML(tinyxml2::XMLPrinter & xml);
			virtual void decodeXML(tinyxml2::XMLElement & xml);
			
			Sprite background;
			void updateSprite();
			
			void Render(sf::RenderTarget & target) const;
		};
	}
}
