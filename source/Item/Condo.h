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

			/**
			 * A Person living in a Condo item. The class maintains
			 * additional information of the occupant, such as the day's schedule
			 * and the like.
			 */
			class CondoOccupant : public Person {
			public:
				CondoOccupant(Condo *item, Person::Type type, double depart, double ret)
					: Person(item->game, type),
					departureTime(depart),
					returnTime(ret),
					departureJitter(0.0),
					returnJitter(0.0),
					atHome(true)
					{}

				/// When the occupant is supposed to leave the tower in the morning.
				double departureTime;
				/// When the occupant is supposed to return to the tower in the evening.
				double returnTime;

				/// The difference between when they are supposed to leave and when they actually will
				double departureJitter;
				/// The difference between when they are supposed to return and when they actually will
				double returnJitter;
				/// True if the person is currently in the condo
				bool atHome;

				struct departsLaterThan : public std::binary_function<CondoOccupant *, CondoOccupant *, bool> {
					bool operator() (const CondoOccupant * a, const CondoOccupant * b) const {
						return (a->actualDepartureTime() > a->actualDepartureTime());
					}
				};
				struct returnsLaterThan : public std::binary_function<CondoOccupant *, CondoOccupant *, bool> {
					bool operator() (const CondoOccupant * a, const CondoOccupant * b) const {
						return (a->actualReturnTime() > a->actualReturnTime());
					}
				};

				double actualReturnTime() const;
				double actualDepartureTime() const;
			};

			virtual void init();

			virtual void encodeXML(tinyxml2::XMLPrinter & xml) override;
			virtual void decodeXML(tinyxml2::XMLElement & xml) override;
			virtual void advance(double dt);

			virtual void addPerson(Person * p);
			virtual void removePerson(Person * p);
		protected:
			void generateJitters();
			void moveOccupants();
			void createOccupants();
			void removeOccupants();
			bool updateLighting(double time);
			void updateSprite();
			bool isAttractive();

			int rent;
			int rentDeposit;
			int variant;
			LightingConditions lighting;
			bool occupied;

			Sprite sprite;
			bool spriteNeedsUpdate;
			std::set<CondoOccupant*> occupants;
			std::priority_queue<CondoOccupant *, std::deque<CondoOccupant *>, CondoOccupant::departsLaterThan> departureQueue;
			std::priority_queue<CondoOccupant *, std::deque<CondoOccupant *>, CondoOccupant::returnsLaterThan> returnQueue;
		};
	}
}
