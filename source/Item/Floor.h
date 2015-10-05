#pragma once
#include <set>
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
			Sprite ceiling;
			void updateSprite();

			/*
				Interval stores a sorted list of min/max X positions of
				building items on this floor (including its own).

				Every overlapping pair of values in this list determines an
				segment interval of the floor that corresponds to either of
				the following:
					a)Odd-numbered pair (1st, 3rd, 5th...) -> Full floor sprite
					b)Even-numbered pair (2nd, 4th, 6th...)-> Ceiling sprite
			*/
			std::multiset<int> interval;
			void render(sf::RenderTarget & target) const;
		};
	}
}
