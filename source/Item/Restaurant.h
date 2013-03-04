#pragma once
#include "../Sprite.h"
#include "Item.h"

namespace OT {
	namespace Item {
		class Restaurant : public Item
		{
		public:
			OT_ITEM_CONSTRUCTOR(Restaurant);
			OT_ITEM_PROTOTYPE(Restaurant) {
				p->id    = "restaurant";
				p->name  = "Restaurant";
				p->price = 200000;
				p->size  = int2(24,1);
				p->icon  = 12;
			}
			virtual ~Restaurant();
			
			virtual void init();
			
			virtual void encodeXML(tinyxml2::XMLPrinter & xml);
			virtual void decodeXML(tinyxml2::XMLElement & xml);
			
			int variant;
			bool open;
			
			class Customer : public Person {
			public:
				Customer(Restaurant * item);
				virtual ~Customer()	{ LOG(DEBUG, "restaurant customer %p killed", this); }
				double arrivalTime; //when the customer arrives at the lobby of the tower

				struct laterThan : public std::binary_function<Customer *, Customer *, bool> {
					bool operator() (const Customer * _Left, const Customer * _Right) const {
						return (_Left->arrivalTime > _Right->arrivalTime);
					}
				};
			};
			std::priority_queue<Customer *, std::deque<Customer *>, Customer::laterThan> arrivingCustomers; // Priority queue to facilitate selection of customers spawned by Restaurant due to arrive at tower
			std::list<Person *> eatingCustomers; // List (in order of arrival) to facilitate selection of customers due to leave Restaurant
			typedef std::set<Customer *> Customers;
			Customers customers;
			void clearCustomers();
			
			struct CustomerMetadata {
				double arrivalTime;   //when the customer arrived at the Restaurant
			};
			typedef std::map<Person *, CustomerMetadata> CustomerMetadataMap;
			CustomerMetadataMap customerMetadata;
			
			Sprite sprite;
			bool spriteNeedsUpdate;
			void updateSprite();
			
			virtual void advance(double dt);
			
			virtual void addPerson(Person * p);
			virtual void removePerson(Person * p);

			Path getRandomBackgroundSoundPath();
		};
	}
}
