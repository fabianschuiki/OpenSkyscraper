#include "singleroom.h"

using namespace OSS;
using namespace Classic;


ItemDescriptor SingleRoomItem::descriptor = {
	kSingleRoomType,
	kHotelGroup,
	kFacilityCategory,
	1,
	(kNotBelowGroundAttribute),
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

string SingleRoomItem::getTypeName()
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
