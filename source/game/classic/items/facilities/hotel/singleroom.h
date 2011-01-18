#ifndef OSS_CLASSIC_ITEMS_FACILITIES_HOTEL_SINGLEROOM_H
#define OSS_CLASSIC_ITEMS_FACILITIES_HOTEL_SINGLEROOM_H

#include "hotel.h"


namespace OSS {
	namespace Classic {
		class SingleRoomItem : public HotelItem {
		public:
			static ItemDescriptor descriptor;
			
			//Initialization
			SingleRoomItem(Tower * tower);
			virtual string getTypeName() const { return "single"; }
			
			//Guests
			unsigned int getMaxNumberOfGuests();
		};
	}
}


#endif
