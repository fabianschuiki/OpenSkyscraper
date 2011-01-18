#ifndef OSS_CLASSIC_PEOPLE_INDIVIDUALS_HOTELGUEST_H
#define OSS_CLASSIC_PEOPLE_INDIVIDUALS_HOTELGUEST_H

#include "../person.h"

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
			virtual string getTypeName() const { return "hotel/guest"; }
			
			/**
			 * Animation
			 */
			virtual void updateAnimation();
			virtual bool shouldAnimate();
			
			/**
			 * Simulation
			 */
		private:
			bool initialVisitDone;
			bool hadDinner;
			double sleepTime;
			bool didChooseSleepTime;
			bool asleep;
			bool didSleep;
			bool didChooseCheckoutTime;
			bool checkingOut;
			
		public:
			bool isAsleep();
			void setAsleep(bool asleep);
			
			bool isCheckingOut();
			
			virtual void think();
		};
	}
}


#endif
