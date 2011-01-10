#ifndef OSS_CLASSIC_ITEMS_TRANSPORTS_STAIRSLIKE_STAIRS_H
#define OSS_CLASSIC_ITEMS_TRANSPORTS_STAIRSLIKE_STAIRS_H

#include "stairslike.h"


namespace OSS {
	namespace Classic {
		class StairsItem : public StairslikeItem {
		public:
			static ItemDescriptor descriptor;
			
			//Initialization
			StairsItem(Tower * tower);
			
			//Basic Sprites
			void updateBackground();
		};
	}
}


#endif
