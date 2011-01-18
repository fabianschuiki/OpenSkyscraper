#ifndef OSS_CLASSIC_ITEMS_TRANSPORTS_ELEVATORS_ELEVATOR_H
#define OSS_CLASSIC_ITEMS_TRANSPORTS_ELEVATORS_ELEVATOR_H

#include "../transport.h"


namespace OSS {
	namespace Classic {
		class ElevatorCar;
		class ElevatorQueue;
		
		class ElevatorItem : public TransportItem {
			
			/**
			 * Initialization
			 */
		public:
			ElevatorItem(Tower * tower, ItemDescriptor * descriptor);
			virtual string getMotorBufferTypeName() const { return "standard"; }
			
			
			/**
			 * Queues
			 */
		public:
			typedef enum {
				kUp = 1,
				kDown = -1
			} Direction;
			
		private:
			typedef map<Direction, Pointer<ElevatorQueue> > QueuePair;
			typedef map<int, QueuePair> QueueMap;
			QueueMap queues;
			
		public:
			ElevatorQueue * getQueue(int floor, Direction dir);
				
			
			/**
			 * Layout
			 */
		public:
			virtual bool isFloorActive(int floor);
			virtual bool isFloorHighlighted(int floor);
			
			recti getMotorRect();
			recti getBufferRect();
			recti getCarsRect();
			
			
			/**
			 * Cars
			 */
		private:
			typedef set< Pointer<ElevatorCar> > CarSet;
			CarSet cars;
			
		public:
			void addCar(ElevatorCar * car);
			void addCar(int floor);
			void removeCar(ElevatorCar * car);
			virtual long getCarPrice() { return 0; }
			
			virtual double maxCarAcceleration() { return 7.5; }
			virtual double maxCarSpeed() { return 10.0; }
			
			virtual void respondToCalls();
			
			Updatable::Conditional<ElevatorItem> respondToCallsIfNeeded;
			
			
			/**
			 * Simulation
			 */
		private:
			unsigned int animationFrame;
			
		public:
			unsigned int getAnimationFrame();
			void setAnimationFrame(unsigned int af);
			
			virtual void advanceItem(double dt);
			
			bool shouldAnimate() { return true; }
			
			
			/**
			 * State
			 */
		public:
			virtual void updateItem();
			virtual void updateBackground();
			virtual void updateQueues();
			virtual void updateQueueLocations();
			
			Updatable::Conditional<ElevatorItem> updateQueuesIfNeeded;
			Updatable::Conditional<ElevatorItem> updateQueueLocationsIfNeeded;
			
			
			/**
			 * Drawing
			 */
		private:
			Pointer<Sprite> motorSprite;
			Pointer<Sprite> bufferSprite;
			
			//Background textures
			Pointer<Texture> backgroundNormal;
			Pointer<Texture> backgroundHighlighted;
			Pointer<Texture> lsNormal;
			Pointer<Texture> lsHighlighted;
			Pointer<Texture> msNormal;
			Pointer<Texture> msHighlighted;
			
		public:
			virtual void drawItem(rectd dirtyRect);
			virtual void drawBackground(rectd dirtyRect);
			virtual void drawFloor(int f);
			virtual void drawFloorBackground(int f, rectd rect);
			virtual void drawFloorNumber(int f, rectd rect);
			virtual void drawCars(rectd dirtyRect);
			virtual void drawQueues(rectd dirtyRect);
			
			
			/**
			 * Events
			 */
		public:
			int debugFloor;
			virtual bool eventKeyDown(KeyEvent * event);
			
			
			/**
			 * People
			 */
		public:
			virtual void didAddPerson(Person * person);
			virtual void willRemovePerson(Person * person);
		};
	}
}


#include "car.h"
#include "queue.h"


#endif
