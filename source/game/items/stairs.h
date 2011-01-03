#ifndef OSS_STAIRS_H
#define OSS_STAIRS_H


#include "../../general.h"
#include "stairslike.h"


namespace OSS {
	class StairsItem : public StairslikeItem {
	public:
		static Item::Descriptor descriptor;
		
		//Initialization
		StairsItem(Tower * tower);
		
		//Basic Sprites
		void updateBackground();
	};
}


#endif
