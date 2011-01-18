#include "express.h"

using namespace OSS;
using namespace Classic;


ItemDescriptor ExpressElevatorItem::descriptor = {
	kExpressElevatorType,
	kElevatorGroup,
	kTransportCategory,
	1,
	(0),
	400000,
	int2(6, 1)
};





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

ExpressElevatorItem::ExpressElevatorItem(Tower * tower) : ElevatorItem(tower, &descriptor)
{
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Layout
//----------------------------------------------------------------------------------------------------

bool ExpressElevatorItem::isFloorActive(int floor)
{
	return (ElevatorItem::isFloorActive(floor) && (floor % 15 == 0));
}
