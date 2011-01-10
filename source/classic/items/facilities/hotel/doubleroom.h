#ifndef OSS_CLASSIC_ITEMS_FACILITIES_HOTEL_DOUBLEROOM_H
#define OSS_CLASSIC_ITEMS_FACILITIES_HOTEL_DOUBLEROOM_H

#include "hotel.h"


namespace OSS {
	namespace Classic {
		class DoubleRoomItem : public HotelItem {
		public:
			static ItemDescriptor descriptor;
			
			//Initialization
			DoubleRoomItem(Tower * tower);
			std::string getTypeName();
		};
	}
}


#endif
