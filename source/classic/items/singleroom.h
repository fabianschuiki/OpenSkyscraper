#ifndef OSS_SINGLEROOM_H
#define OSS_SINGLEROOM_H


#include "../../general.h"
#include "hotelitem.h"


namespace OSS {
	class SingleRoomItem : public HotelItem {
	public:
		static Item::Descriptor descriptor;
		
		//Initialization
		SingleRoomItem(Tower * tower);
		std::string getTypeName();
		
		//Guests
		unsigned int getMaxNumberOfGuests();
	};
}


#endif
