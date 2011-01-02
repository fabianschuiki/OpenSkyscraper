#ifndef OSS_OFFICE_H
#define OSS_OFFICE_H

#include "../../general.h"
#include "../item.h"


namespace OSS {
	class OfficeItem : public Item {
	public:
		static Item::Descriptor descriptor;
		
		unsigned int type;
		bool vacant;
		
		OfficeItem();
		
		void updateBackground();
		void advance(double dt);
	};
}


#endif
