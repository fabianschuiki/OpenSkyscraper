/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "Item.h"

namespace OT {
	namespace Item {
		class Office : public Item
		{
		public:
			OT_ITEM_CONSTRUCTOR(Office);
			OT_ITEM_PROTOTYPE(Office) {
				p->id    = "office";
				p->name  = "Office";
				p->price = 40000;
				p->size  = int2(9,1);
				p->icon  = 7;
			}
			virtual ~Office();
			
			virtual void init();
			
			virtual void encodeXML(tinyxml2::XMLPrinter & xml);
			virtual void decodeXML(tinyxml2::XMLElement & xml);
			
			int rent;
			int rentDeposit;
			bool occupied;
			int variant;
			bool lit;
			
			Sprite sprite;
			bool spriteNeedsUpdate;
			void updateSprite();
			
			virtual void advance(double dt);
			bool isAttractive();

			Path getRandomBackgroundSoundPath();
		};
	}
}
