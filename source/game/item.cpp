#include "item.h"
#include "items.h"

using namespace OSS;


static Item::Descriptor floorItemDescriptor = {
	Item::kFloorType,
	Item::kStructureGroup,
	Item::kFacilityCategory,
	1,
	(Item::kFlexibleWidthAttribute),
	500,
	int2(1, 1),
	int2(1, 1)
};





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

Item * Item::createNew(Item::Descriptor * descriptor, recti rect, unsigned int itemID)
{
	return NULL;
}

Item::~Item()
{
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Descriptors
//----------------------------------------------------------------------------------------------------

Item::Descriptor * Item::descriptorForItemType(Item::Type itemType)
{
	switch (itemType) {
		case Item::kLobbyType:	return &LobbyItem::descriptor; break;
		case Item::kFloorType:	return &floorItemDescriptor; break;
	}
	return NULL;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Rendering
//----------------------------------------------------------------------------------------------------

void Item::draw()
{
}
