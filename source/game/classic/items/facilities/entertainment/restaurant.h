#ifndef OSS_CLASSIC_ITEMS_FACILITIES_ENTERTAINMENT_RESTAURANT_H
#define OSS_CLASSIC_ITEMS_FACILITIES_ENTERTAINMENT_RESTAURANT_H

#include "../facility.h"


namespace OSS {
	namespace Classic {
		class RestaurantItem : public FacilityItem {
			
			/**
			 * Initialization
			 */
		public:
			static ItemDescriptor descriptor;
			
			RestaurantItem(Tower * tower);
			virtual string getTypeName() const { return "restaurant"; }
			
			
			/**
			 * State
			 */
		public:
			virtual string getAutoTextureName(int floor, unsigned int slice);
			virtual rectd getAutoTextureRect(int floor, unsigned int slice);
		};
	}
}


#endif
