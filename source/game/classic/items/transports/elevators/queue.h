#ifndef OSS_CLASSIC_ITEMS_TRANSPORTS_ELEVATORS_QUEUE_H
#define OSS_CLASSIC_ITEMS_TRANSPORTS_ELEVATORS_QUEUE_H

#include "elevator.h"


namespace OSS {
	namespace Classic {
		class ElevatorQueue : public GameObject {
			
			/**
			 * Initialization
			 */
		public:
			const Pointer<ElevatorItem> elevator;
			
			ElevatorQueue(ElevatorItem * elevator);
			
			
			/**
			 * Layout
			 */
		private:
			recti rect;
			ElevatorItem::Direction direction;
			
		public:
			const recti & getRect() const;
			void setRect(const recti & r);
			
			rectd getWorldRect() const;
			
			ElevatorItem::Direction getDirection();
			void setDirection(ElevatorItem::Direction dir);
			
			
			/**
			 * People
			 */
		private:
			typedef list<Person *> PersonList;
			PersonList people;
			bool steppingInside;
			
		public:
			void addPerson(Person * p);
			void removePerson(Person * p);
			
			bool hasPeople();
			Person * popPerson();
			
			bool isSteppingInside();
			void setSteppingInside(bool si);
			
			
			/**
			 * Response
			 */
		private:
			bool called;
			double callTime;
			Pointer<ElevatorCar> respondingCar;
			
		public:
			bool isCalled();
			bool isCallAnswered();
			double getWaitDuration();
			
			void callCar();
			void clearCall();
			
			
			/**
			 * State
			 */
		public:
			virtual void update();
			virtual void updateSprites();
			
			Updatable::Conditional<ElevatorQueue> updateSpritesIfNeeded;
			
			
			/**
			 * Drawing
			 */
		private:
			typedef map<Person *, Pointer<Sprite> > PersonSpriteMap;
			PersonSpriteMap personSprites;
			
		public:
			virtual void draw(rectd dirtyRect);
		};
	}
}


#endif
