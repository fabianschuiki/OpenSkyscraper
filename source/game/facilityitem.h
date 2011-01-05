#ifndef OSS_FACILITYITEM_H
#define OSS_FACILITYITEM_H


#include "../general.h"
#include "item.h"


namespace OSS {
	class FacilityItem : public Item {
	public:
		//Initialization
		FacilityItem(Tower * tower, Item::Descriptor * descriptor);
		
		//Basic Sprites
		void initBasicSprites();
		void updateBasicSprites();
		
		Sprite ceiling;
		bool hasCeiling;
		virtual void initCeiling();
		virtual void updateCeiling();
		virtual void updateBackground();
		
		//Drawing
		virtual void draw(rectd visibleRect);
	};
}


#endif
