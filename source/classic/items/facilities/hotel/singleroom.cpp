#include "singleroom.h"
#include "../tower.h"

using namespace OSS;


Item::Descriptor SingleRoomItem::descriptor = {
	Item::kSingleRoomType,
	Item::kHotelGroup,
	Item::kFacilityCategory,
	1,
	(Item::kNotBelowGroundAttribute),
	20000,
	int2(4, 1)
};





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

SingleRoomItem::SingleRoomItem(Tower * tower) : HotelItem(tower, &descriptor)
{
	setVariant(randui(0, 1));
}

std::string SingleRoomItem::getTypeName()
{
	return "single";
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

unsigned int SingleRoomItem::getMaxNumberOfGuests()
{
	return 1;
}
