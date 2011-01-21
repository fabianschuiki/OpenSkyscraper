#ifndef OSS_CLASSIC_ITEMS_FACILITIES_HOTEL_HOUSEKEEPING_H
#define OSS_CLASSIC_ITEMS_FACILITIES_HOTEL_HOUSEKEEPING_H

#include "../facility.h"


namespace OSS {
	namespace Classic {
		class Janitor;
		
		class HousekeepingItem : public FacilityItem {
			
			/**
			 * Initialization
			 */
		public:
			static ItemDescriptor descriptor;
			
			HousekeepingItem(Tower * tower);
			virtual string getTypeName() const { return "housekeeping"; }
			
			
			/**
			 * Janitors
			 */
		private:
			typedef std::set< Pointer<Janitor> > JanitorSet;
			JanitorSet janitors;
			
		public:
			void onJanitorDone(Janitor * janitor);
			virtual void didChangeRect();
			
			
			/**
			 * State
			 */
		public:
			virtual void updateBackground();
			
			
			/**
			 * Events
			 */
		public:
			virtual void eventHotelVacated(ItemEvent<HotelItem> * event);
		};
	}
}


#include "../../../people/individuals/janitor.h"


#endif
