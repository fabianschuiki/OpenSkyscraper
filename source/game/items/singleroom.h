#ifndef OSS_SINGLEROOM_H
#define OSS_SINGLEROOM_H


#include "../../general.h"
#include "../facilityitem.h"


namespace OSS {
	class SingleRoomItem : public FacilityItem {
	public:
		static Item::Descriptor descriptor;
		
		//Initialization
	public:
		SingleRoomItem(Tower * tower);
		
		//Type
	private:
		unsigned int type;
	public:
		unsigned int getType();
		void setType(const unsigned int type);
		
		//Basic Sprites
		void updateBackground();
	};
}


#endif
