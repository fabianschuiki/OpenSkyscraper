#include "suite.h"
#include "../tower.h"

using namespace OSS;


Item::Descriptor SuiteItem::descriptor = {
	Item::kSuiteType,
	Item::kHotelGroup,
	Item::kFacilityCategory,
	1,
	(Item::kNotBelowGroundAttribute),
	100000,
	int2(10, 1)
};





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

SuiteItem::SuiteItem(Tower * tower) : HotelItem(tower, &descriptor)
{
	setVariant(randui(0, 1));
}

std::string SuiteItem::getTypeName()
{
	return "suite";
}
