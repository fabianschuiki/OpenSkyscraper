#ifndef OSS_CLASSIC_ITEMS_TRANSPORTS_ELEVATORS_CAR_H
#define OSS_CLASSIC_ITEMS_TRANSPORTS_ELEVATORS_CAR_H

#include "elevator.h"


namespace OSS {
	namespace Classic {
		class ElevatorCar : public GameObject {
			
			/**
			 * Initialization
			 */
		public:
			const Pointer<ElevatorItem> elevator;
			
			ElevatorCar(ElevatorItem * elevator);
			
			
			/**
			 * People
			 */
		private:
			typedef set<Person *> PersonSet;
			PersonSet passengers;
			
		public:
			void addPassenger(Person * p);
			void removePassenger(Person * p);
			bool hasPassengers();
			
			Person * nextPassengerToUnmount();
			int closestPassengerDestinationFloor();
			bool isFull();
			
			
			/**
			 * Simulation
			 */
		private:
			double floor;
			ElevatorItem::Direction direction;
			
			double startFloor;
			int destinationFloor;
			double journeyTime;
			
			Pointer<SoundEffect> arrivingSound;
			Pointer<SoundEffect> departingSound;
			
			bool arrivingPlayed;
			bool departingPlayed;
			
			typedef enum {
				kIdle,
				kMoving,
				kOpeningDoors,
				kHauling,
				kClosingDoors
			} State;
			State state;
			
		public:
			double getFloor();
			void setFloor(double f);
			
			ElevatorItem::Direction getDirection();
			void setDirection(ElevatorItem::Direction dir);
			
			int getDestinationFloor();
			void setDestinationFloor(int f);
			
			State getState();
			void setState(State s);
			bool isIdle();
			
			virtual void advance(double dt);
			
			void decideDestination();
			void answerCall(ElevatorQueue * q);
			
			
			/**
			 * State
			 */
		public:
			unsigned int getTextureSliceIndex();
			
			virtual void update();
			virtual void updateTexture();
			virtual void updatePosition();
			
			Updatable::Conditional<ElevatorCar> updateTextureIfNeeded;
			Updatable::Conditional<ElevatorCar> updatePositionIfNeeded;
			
			
			/**
			 * Drawing
			 */
		private:
			Pointer<Sprite> sprite;
			
		public:
			virtual void draw(rectd dirtyRect);
		};
	}
}


#endif
