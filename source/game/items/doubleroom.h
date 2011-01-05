#ifndef OSS_DOUBLEROOM_H
#define OSS_DOUBLEROOM_H


#include "../../general.h"
#include "hotelitem.h"


namespace OSS {
	class DoubleRoomItem : public HotelItem {
	public:
		static Item::Descriptor descriptor;
		
		//Initialization
		DoubleRoomItem(Tower * tower);
		std::string getTypeName();
	};
}


#endif
