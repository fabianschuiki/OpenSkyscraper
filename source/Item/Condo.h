#pragma once
#include "../Sprite.h"
#include "Item.h"

namespace OT {
	namespace Item {
		class Condo : public Item
		{
		public:
			OT_ITEM_CONSTRUCTOR(Condo);
			OT_ITEM_PROTOTYPE(Condo) {
				p->id    = "condo";
				p->name  = "Condo";
				p->price = 200000;
				p->size  = int2(16,1);
				p->icon  = 24;
			}
			virtual ~Condo();

			enum LightingConditions {
				NIGHT,
				LIT,
				DAYTIME
			};
			
			virtual void init();
			
			virtual void encodeXML(tinyxml2::XMLPrinter & xml);
			virtual void decodeXML(tinyxml2::XMLElement & xml);
			bool isAttractive();
			
			int rent;
			int rentDeposit;
			int variant;
			LightingConditions lighting;
			bool occupied;
			
			Sprite sprite;
			bool spriteNeedsUpdate;
			void updateSprite();
			
			virtual void advance(double dt);
			
			virtual void addPerson(Person * p);
			virtual void removePerson(Person * p);
		protected:
			bool updateLighting(double time);
		};
	}
}
