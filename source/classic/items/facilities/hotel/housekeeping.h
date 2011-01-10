#ifndef OSS_CLASSIC_ITEMS_FACILITIES_HOTEL_HOUSEKEEPING_H
#define OSS_CLASSIC_ITEMS_FACILITIES_HOTEL_HOUSEKEEPING_H

#include "../facility.h"

#include "../../../people/individuals/janitor.h"


namespace OSS {
	namespace Classic {
		class HousekeepingItem : public FacilityItem {
		public:
			static ItemDescriptor descriptor;
			
			//Initialization
			HousekeepingItem(Tower * tower);
			void init();
			
			//Basic sprites
			void initBackground();
			
			//Simulation
			void advance(double dt);
			
			//Janitors
		private:
			typedef std::set< Pointer<Janitor> > JanitorSet;
			JanitorSet janitors;
		public:
			void initJanitors();
			void updateJanitors();
			void onJanitorDone(Janitor * janitor);
			
			//Events
			virtual void eventHotelVacated(HotelItem * hotel);
		};
	}
}


#endif
