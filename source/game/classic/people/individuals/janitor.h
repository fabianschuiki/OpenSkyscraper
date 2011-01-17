#ifndef OSS_CLASSIC_PEOPLE_INDIVIDUALS_JANITOR_H
#define OSS_CLASSIC_PEOPLE_INDIVIDUALS_JANITOR_H

#include "../person.h"

#include "../../items/facilities/hotel/hotel.h"
#include "../../items/facilities/hotel/housekeeping.h"


namespace OSS {
	namespace Classic {		
		class Janitor : public Person {
			
			/**
			 * Initialization
			 */
		public:
			const Pointer<HousekeepingItem> housekeeping;
			
			Janitor(Tower * tower, HousekeepingItem * housekeeping);
			virtual string getTypeName() { return "hotel/janitor"; }
			
			
			/**
			 * Hotel
			 */
		private:
			Pointer<HotelItem> assignedHotel;
			
		public:
			HotelItem * getAssignedHotel();
			void setAssignedHotel(HotelItem * hotel);
			bool hasAssignedHotel();
			
			virtual void willChangeAssignedHotel();
			virtual void didChangeAssignedHotel();
			
			
			/**
			 * Animation
			 */
		public:
			virtual void updateAnimation();
			virtual bool shouldAnimate();
			
			
			/**
			 * Simulation
			 */
		private:
			bool cleaningDone;
			
		public:
			virtual void think();
		};
	}
}


#endif
