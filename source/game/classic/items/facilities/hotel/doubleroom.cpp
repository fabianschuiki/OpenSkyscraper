#include "doubleroom.h"

using namespace OSS;
using namespace Classic;


ItemDescriptor DoubleRoomItem::descriptor = {
	kDoubleRoomType,
	kHotelGroup,
	kFacilityCategory,
	1,
	(kNotBelowGroundAttribute),
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

string DoubleRoomItem::getTypeName()
{
	return "double";
}
