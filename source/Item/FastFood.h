#pragma once
#include "../Sprite.h"
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
			virtual ~FastFood();
			
			virtual void init();
			
			virtual void encodeXML(tinyxml2::XMLPrinter & xml);
			virtual void decodeXML(tinyxml2::XMLElement & xml);
			
			int variant;
			bool open;
			
			class Customer : public Person {
			public:
				Customer(FastFood * item) : Person(item->game) { arrivalTime = 0; }
				virtual ~Customer()	{ LOG(DEBUG, "fastfood customer %p killed", this); }
				double arrivalTime; //when the customer arrives at the lobby of the tower
			};
			typedef std::set<Customer *> Customers;
			Customers customers;
			void clearCustomers();
			
			struct CustomerMetadata {
				double arrivalTime; //when the customer arrived at the fast food
			};
			typedef std::map<Person *, CustomerMetadata> CustomerMetadataMap;
			CustomerMetadataMap customerMetadata;
			
			Sprite sprite;
			bool spriteNeedsUpdate;
			void updateSprite();
			
			virtual void advance(double dt);
			
			virtual void addPerson(Person * p);
			virtual void removePerson(Person * p);
		};
	}
}
