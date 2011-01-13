#ifndef OSS_CLASSIC_PEOPLE_INDIVIDUALS_HOTELGUEST_H
#define OSS_CLASSIC_PEOPLE_INDIVIDUALS_HOTELGUEST_H

#include "../timedperson.h"


namespace OSS {
	namespace Classic {
		class HotelItem;
		
		class HotelGuest : public TimedPerson {
		public:
			const Pointer<HotelItem> hotel;
			
			//Initialization
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
			bool isLeaving();
			
			//Sleep
		private:
			bool asleep;
		public:
			bool isAsleep();
			void setAsleep(bool asleep);
			
			//Animation Sprite
			void initAnimationSprite();
			void updateAnimationSprite();
			
			bool shouldAnimate();
			void shuffleAnimation();
		};
	}
}


#endif
