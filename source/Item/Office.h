/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "Item.h"
#include <set>

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

			virtual void addPerson(Person * p);

			Path getRandomBackgroundSoundPath();

			/**
			 * A Person working in an Office item. The class maintains
			 * additional information of the worker, such as the day's schedule
			 * and the like.
			 */
			class Worker : public Person {
			public:
				Worker(Office *item, Person::Type type) : Person(item->game) { this->type = type; }

				/// When the worker is supposed to arrive at the tower in the morning.
				double arrivalTime;
				/// When the worker is supposed to leave the tower in the evening.
				double departureTime;

				/// In case of a salesman, stores when he will leave the tower
				/// to go on his sales trip.
				double leaveForSalesTime; //absolute
				/// In case of a salesman, stores when he will return from his
				/// sales trip.
				double returnFromSalesTime; //absolute

				/// In case of a regular worker, stores when the worker is
				/// supposed to have lunch.
				double lunchTime;


				struct arrivesLaterThan : public std::binary_function<Worker *, Worker *, bool> {
					bool operator() (const Worker * a, const Worker * b) const {
						return (a->arrivalTime > a->arrivalTime);
					}
				};
				struct departsLaterThan : public std::binary_function<Worker *, Worker *, bool> {
					bool operator() (const Worker * a, const Worker * b) const {
						return (a->departureTime > a->departureTime);
					}
				};
				struct lunchLaterThan : public std::binary_function<Worker *, Worker *, bool> {
					bool operator() (const Worker * a, const Worker * b) const {
						return (a->lunchTime > a->lunchTime);
					}
				};
				struct leaveForSalesLaterThan : public std::binary_function<Worker *, Worker *, bool> {
					bool operator() (const Worker * a, const Worker * b) const {
						return (a->leaveForSalesTime > a->leaveForSalesTime);
					}
				};
				struct returnFromSalesLaterThan : public std::binary_function<Worker *, Worker *, bool> {
					bool operator() (const Worker * a, const Worker * b) const {
						return (a->returnFromSalesTime > a->returnFromSalesTime);
					}
				};
			};
			typedef std::set<Worker *> Workers;
			Workers workers;

			void rescheduleWorkers();

		private:
			std::priority_queue<Worker *, std::deque<Worker *>, Worker::arrivesLaterThan> arrivalQueue;
			std::priority_queue<Worker *, std::deque<Worker *>, Worker::departsLaterThan> departureQueue;
			std::priority_queue<Worker *, std::deque<Worker *>, Worker::lunchLaterThan> lunchQueue;
			std::priority_queue<Worker *, std::deque<Worker *>, Worker::leaveForSalesLaterThan> salesLeaveQueue;
			std::priority_queue<Worker *, std::deque<Worker *>, Worker::returnFromSalesLaterThan> salesReturnQueue;

			Workers workersToArrive;
			Workers workersToDepart;
			Workers workersToHaveLunch;
		};
	}
}
