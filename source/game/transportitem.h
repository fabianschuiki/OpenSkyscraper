#ifndef OSS_TRANSPORTITEM_H
#define OSS_TRANSPORTITEM_H

#include "../general.h"
#include "item.h"


namespace OSS {
	class TransportItem : public Item {
	public:
		//Initialization
		TransportItem(Tower * tower, Item::Descriptor * descriptor) : Item(tower, descriptor) {}
	};
}


#endif
