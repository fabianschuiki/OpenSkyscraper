#include "doubleroom.h"
#include "../tower.h"

using namespace OSS;


Item::Descriptor DoubleRoomItem::descriptor = {
	Item::kDoubleRoomType,
	Item::kHotelGroup,
	Item::kFacilityCategory,
	1,
	(Item::kNotBelowGroundAttribute),
	50000,
	int2(6, 1)
};





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

DoubleRoomItem::DoubleRoomItem(Tower * tower) : HotelItem(tower, &descriptor)
{
	setVariant(randui(0, 3));
}

std::string DoubleRoomItem::getTypeName()
{
	return "double";
}
