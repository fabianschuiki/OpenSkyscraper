#ifndef OSS_ESCALATOR_H
#define OSS_ESCALATOR_H


#include "../../general.h"
#include "stairslike.h"


namespace OSS {
	class EscalatorItem : public StairslikeItem {
	public:
		static Item::Descriptor descriptor;
		
		//Initialization
		EscalatorItem(Tower * tower);
	};
}


#endif
