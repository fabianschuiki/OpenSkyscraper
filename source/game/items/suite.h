#ifndef OSS_SUITE_H
#define OSS_SUITE_H


#include "../../general.h"
#include "hotelitem.h"


namespace OSS {
	class SuiteItem : public HotelItem {
	public:
		static Item::Descriptor descriptor;
		
		//Initialization
		SuiteItem(Tower * tower);
		std::string getTypeName();
	};
}


#endif
