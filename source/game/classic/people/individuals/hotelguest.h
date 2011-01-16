#ifndef OSS_CLASSIC_PEOPLE_INDIVIDUALS_HOTELGUEST_H
#define OSS_CLASSIC_PEOPLE_INDIVIDUALS_HOTELGUEST_H

#include "../timedperson.h"
#include "../../items/facilities/hotel/hotel.h"


namespace OSS {
	namespace Classic {
		class HotelGuest : public Person {
			
			/**
			 * Initialization
			 */
		public:
			const Pointer<HotelItem> hotel;
			
			HotelGuest(Tower * tower, HotelItem * hotel);
			virtual string getTypeName() { return "hotel/guest"; }
			
			//Intelligence
		private:
			bool initialVisitDone;
			bool hadDinner;
			double sleepTime;
			bool didChooseSleepTime;
			bool didSleep;
			bool checkingOut;
		public:
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
			 * Simulation
			 */
		public:
			virtual void think();
		};
	}
}


#endif
