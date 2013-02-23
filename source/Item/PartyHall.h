#pragma once
#include "../Sprite.h"
#include "Item.h"

namespace OT {
	namespace Item {
		class PartyHall : public Item
		{
		public:
			OT_ITEM_CONSTRUCTOR(PartyHall);
			OT_ITEM_PROTOTYPE(PartyHall) {
				p->id    = "partyhall";
				p->name  = "Party Hall";
				p->price = 500000;
				p->size  = int2(24,2);
				p->icon  = 15;
			}
			virtual ~PartyHall();
			
			virtual void init();
			
			virtual void encodeXML(tinyxml2::XMLPrinter & xml);
			virtual void decodeXML(tinyxml2::XMLElement & xml);
			
			bool open;
			
			Sprite sprite;
			bool spriteNeedsUpdate;
			void updateSprite();
			
			virtual void advance(double dt);

			Path getRandomBackgroundSoundPath();
		};
	}
}
