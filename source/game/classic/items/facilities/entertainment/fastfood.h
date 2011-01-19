#ifndef OSS_CLASSIC_ITEMS_FACILITIES_ENTERTAINMENT_FASTFOOD_H
#define OSS_CLASSIC_ITEMS_FACILITIES_ENTERTAINMENT_FASTFOOD_H

#include "../facility.h"


namespace OSS {
	namespace Classic {
		class FastFoodItem : public FacilityItem {
			
			/**
			 * Initialization
			 */
		public:
			static ItemDescriptor descriptor;
			
			FastFoodItem(Tower * tower);
			virtual string getTypeName() const { return "fastfood"; }
			
			
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
