#ifndef OSS_CLASSIC_ITEMS_FACILITIES_ENTERTAINMENT_FASTFOOD_H
#define OSS_CLASSIC_ITEMS_FACILITIES_ENTERTAINMENT_FASTFOOD_H

#include "entertainment.h"


namespace OSS {
	namespace Classic {
		class FastFoodItem : public EntertainmentItem {
			
			/**
			 * Initialization
			 */
		public:
			static ItemDescriptor descriptor;
			
			FastFoodItem(Tower * tower);
			virtual string getTypeName() const { return "fastfood"; }
			
			
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
