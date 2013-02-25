#pragma once
#include "../Sprite.h"
#include "Item.h"
#include <set>

namespace OT {
	namespace Item {
		class Cinema : public Item
		{
		public:
			OT_ITEM_CONSTRUCTOR(Cinema);
			OT_ITEM_PROTOTYPE(Cinema) {
				p->id    = "cinema";
				p->name  = "Movie Theatre";
				p->price = 500000;
				p->size  = int2(31,2);
				p->icon  = 14;
				p->entrance_offset = 1;
			}
			virtual ~Cinema();
			
			virtual void init();
			
			virtual void encodeXML(tinyxml2::XMLPrinter & xml);
			virtual void decodeXML(tinyxml2::XMLElement & xml);
			
			bool open;
			bool playing;
			int movieType;

			class Customer : public Person {
			public:
				Customer(Cinema *cinema);
			};
			typedef std::set<Customer *> Customers;
			Customers customers;
			void clearCustomers();
			
			double animation;
			int animationFrame;
			Sprite hallSprite;
			Sprite screenSprite;
			bool spriteNeedsUpdate;
			void updateSprite();
			
			virtual void advance(double dt);

			void addPerson(Person * p);
			void removePerson(Person * p);

			Path getRandomBackgroundSoundPath();
		};
	}
}
