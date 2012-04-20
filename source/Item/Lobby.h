#pragma once
#include "../Sprite.h"
#include "Item.h"

namespace OT {
	namespace Item {
		class Lobby : public Item
		{
		public:
			OT_ITEM_CONSTRUCTOR(Lobby);
			OT_ITEM_PROTOTYPE(Lobby) {
				p->id    = "lobby";
				p->name  = "Lobby";
				p->price = 20000;
				p->size  = int2(4,1);
				p->icon  = 0;
			}
			virtual ~Lobby();
			
			virtual void init();
			
			virtual void encodeXML(tinyxml2::XMLPrinter & xml);
			virtual void decodeXML(tinyxml2::XMLElement & xml);
			
			Sprite background;
			Sprite overlay;
			Sprite entrances[2];
			void updateSprite();
			
			void Render(sf::RenderTarget & target) const;
		};
	}
}
