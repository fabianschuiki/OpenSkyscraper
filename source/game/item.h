#ifndef OSS_ITEM_H
#define OSS_ITEM_H

#include "../general.h"
#include "../base/base.h"
#include "../core/core.h"


namespace OSS {
	class Item : public CoreObject {
	public:
		//Item Types
		typedef enum {
			kNoneType = 0,
			
			kLobbyType,
			kFloorType
		} Type;
		
		//Item Groups
		typedef enum {
			kStructureGroup,
			kElevatorGroup,
			kOfficeGroup,
			kHotelGroup,
			kEntertainmentGroup,
			kInfrastructureGroup,
			kServicesGroup
		} Group;
		
		//Item Categories
		typedef enum {
			kFacilityCategory,
			kTransportCategory
		} Category;
		
		//Item Attributes
		enum {
			kFlexibleWidthAttribute		= (1 << 0),
			kEvery15thFloorAttribute	= (1 << 1),
			kNotAboveGroundAttribute	= (1 << 2),
			kNotBelowGroundAttribute	= (1 << 3)
		};
		
		//Item Descriptor
		typedef struct {
			Type type;
			Group group;
			Category category;
			unsigned short minRating;
			unsigned short attributes;
			double price;
			int2 cells;
			int2 minUnit;
		} Descriptor;
		
		
		//Attributes
		unsigned int itemID;
		Descriptor * descriptor;
		recti rect;
		
		//Initialization
		static Item * createNew(Descriptor * descriptor, recti rect, unsigned int itemID);
		~Item();
		
		//Descriptors
		static Descriptor * descriptorForItemType(Type itemType);
		
		//Rendering
		virtual void draw();
	};
}


#endif
