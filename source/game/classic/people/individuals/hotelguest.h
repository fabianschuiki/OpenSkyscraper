#ifndef OSS_CLASSIC_PEOPLE_INDIVIDUALS_HOTELGUEST_H
#define OSS_CLASSIC_PEOPLE_INDIVIDUALS_HOTELGUEST_H

#include "../timedperson.h"
#include "../../items/facilities/hotel/hotel.h"


namespace OSS {
	namespace Classic {
		class HotelGuest : public TimedPerson {
			
			/**
			 * Initialization
			 */
		public:
			const Pointer<HotelItem> hotel;
			
			HotelGuest(Tower * tower, HotelItem * hotel);
			
			//Intelligence
		private:
			bool initialVisitDone;
			bool hadDinner;
			double sleepTime;
			bool didChooseSleepTime;
			bool didSleep;
		public:
			void think();
			bool isCheckingOut();
			
			//Sleep
		private:
			bool asleep;
		public:
			bool isAsleep();
			void setAsleep(bool asleep);
			
			//Animation Sprite
			/*void initAnimationSprite();
			void updateAnimationSprite();
			
			bool shouldAnimate();
			void shuffleAnimation();*/
			
			/**
			 * State
			 */
		public:
			virtual void updateNextDestination();
		};
	}
}


#endif
