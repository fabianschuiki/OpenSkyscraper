#include "suite.h"

using namespace OSS;
using namespace Classic;


ItemDescriptor SuiteItem::descriptor = {
	kSuiteType,
	kHotelGroup,
	kFacilityCategory,
	1,
	(kNotBelowGroundAttribute),
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
