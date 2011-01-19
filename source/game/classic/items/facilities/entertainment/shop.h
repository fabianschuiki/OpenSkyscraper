#ifndef OSS_CLASSIC_ITEMS_FACILITIES_ENTERTAINMENT_SHOP_H
#define OSS_CLASSIC_ITEMS_FACILITIES_ENTERTAINMENT_SHOP_H

#include "../facility.h"


namespace OSS {
	namespace Classic {
		class ShopItem : public FacilityItem {
			
			/**
			 * Initialization
			 */
		public:
			static ItemDescriptor descriptor;
			
			ShopItem(Tower * tower);
			virtual string getTypeName() const { return "shop"; }
			
			
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
