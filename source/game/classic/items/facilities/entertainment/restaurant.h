#ifndef OSS_CLASSIC_ITEMS_FACILITIES_ENTERTAINMENT_RESTAURANT_H
#define OSS_CLASSIC_ITEMS_FACILITIES_ENTERTAINMENT_RESTAURANT_H

#include "entertainment.h"


namespace OSS {
	namespace Classic {
		class RestaurantItem : public EntertainmentItem {
			
			/**
			 * Initialization
			 */
		public:
			static ItemDescriptor descriptor;
			
			RestaurantItem(Tower * tower);
			virtual string getTypeName() const { return "restaurant"; }
			
			
			/**
			 * Openness
			 */
		public:
			virtual bool shouldBeOpen();
			virtual void didChangeOpenness();
			
			
			/**
			 * State
			 */
		public:
			virtual string getAutoTextureName(int floor, unsigned int slice);
			virtual rectd getAutoTextureRect(int floor, unsigned int slice);
			
			virtual void updateBackground();
		};
	}
}


#endif
