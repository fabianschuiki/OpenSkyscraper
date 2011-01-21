#ifndef OSS_CLASSIC_ITEMS_FACILITIES_ENTERTAINMENT_PARTYHALL_H
#define OSS_CLASSIC_ITEMS_FACILITIES_ENTERTAINMENT_PARTYHALL_H

#include "../facility.h"


namespace OSS {
	namespace Classic {
		class PartyHallItem : public FacilityItem {
			
			/**
			 * Initialization
			 */
		public:
			static ItemDescriptor descriptor;
			
			PartyHallItem(Tower * tower);
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
