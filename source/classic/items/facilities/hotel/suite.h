#ifndef OSS_CLASSIC_ITEMS_FACILITIES_HOTEL_SUITE_H
#define OSS_CLASSIC_ITEMS_FACILITIES_HOTEL_SUITE_H

#include "hotel.h"


namespace OSS {
	namespace Classic {
		class SuiteItem : public HotelItem {
		public:
			static ItemDescriptor descriptor;
			
			//Initialization
			SuiteItem(Tower * tower);
			std::string getTypeName();
		};
	}
}


#endif
