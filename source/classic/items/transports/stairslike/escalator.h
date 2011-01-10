#ifndef OSS_CLASSIC_ITEMS_TRANSPORTS_STAIRSLIKE_ESCALATOR_H
#define OSS_CLASSIC_ITEMS_TRANSPORTS_STAIRSLIKE_ESCALATOR_H

#include "stairslike.h"


namespace OSS {
	namespace Classic {
		class EscalatorItem : public StairslikeItem {
		public:
			static ItemDescriptor descriptor;
			
			//Initialization
			EscalatorItem(Tower * tower);
		};
	}
}


#endif
