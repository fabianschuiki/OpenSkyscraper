#ifndef OSS_CLASSIC_PEOPLE_PERSON_H
#define OSS_CLASSIC_PEOPLE_PERSON_H

#include "../game.h"

#include "../items/item.h"
#include "../tower/tower.h"
#include "../tower/route.h"


namespace OSS {
	namespace Classic {		
		class Person : public GameObject {
			
			/**
			 * Initialization
			 */
		public:
			const Pointer<Tower> tower;

			Person(Tower * tower);
			virtual ~Person();
			
			
			/**
			 * Location
			 */
		private:
			int floor;
			Pointer<Item> item;
			double arrivalTime;
		public:
			int getFloor();
			void setFloor(int f);
			
			Item * getItem() const;
			void setItem(Item * i);
			
			recti getItemRect();
			bool isAt(Item * i);
			
			virtual void willMoveToItem(Item * newItem) {}
			virtual void didMoveToItem() {}
			
			double getArrivalTime();
			void setArrivalTime(double time);
			
			
			/**
			 * Type
			 */
		public:
			typedef enum {
				kManType,
				kSalesmanType,
				kChildType,
				kSecurityOfficerType,
				
				kWomanAType,
				kWomanBType,
				kMotherWithChildType,
				kMotherType,
				kJanitorType
			} Type;
			
		private:
			Type type;
			
		public:
			Type getType();
			void setType(Type t);
			
			virtual void willChangeType(Type newType) {}
			virtual void didChangeType() {}
			
			typedef enum {
				kMale,
				kFemale
			} Gender;
			Gender getGender();
			
			
			/**
			 * Stress
			 */
		private:
			double stress;
		public:
			double getStress();
			void setStress(double s);
			
			virtual void willChangeStress(double newStress) {}
			virtual void didChangeStress() {}
			
			
			/**
			 * Journey
			 */
		private:
			Pointer<Item> destination;
			Pointer<Route> route;
			
			double pauseEndTime;
			
		public:
			Item * getDestination();
			void setDestination(Item * d);
			
			recti getDestinationRect();
			bool isAtDestination();
			
			virtual void willChangeDestination(Item * newDestination) {}
			virtual void didChangeDestination() {}
			
			bool hasRoute();
			Route::Node * getRouteNode();
			int getStartFloor();
			int getEndFloor();
			bool isAtRouteNodeTransport();
			bool isOnStartFloor();
			bool isOnEndFloor();
			
			double getPauseEndTime();
			double getPauseDuration();
			void setPauseEndTime(double t);			//[0, inf[
			void setPauseEndTimeToday(double t);	//[0, 24[
			void setPauseEndTimeTomorrow(double t);	//[0, 24[
			void setPauseDuration(double d);
			
			
			/**
			 * Simulation
			 */
		public:
			virtual void advance(double dt);
			virtual void advanceRoute(double dt);
			virtual void think() {}
			
			virtual bool shouldBeAnimated();
			
			
			/**
			 * State
			 */
		public:
			virtual void update();
			virtual void updateRoute();
			
			Updatable::Conditional<Person> updateRouteIfNeeded;
			
			
			/**
			 * Drawing
			 */
		public:
			virtual void draw(rectd dirtyRect);
			
			virtual bool shouldBeDrawn();
		};
	}
}


#endif
