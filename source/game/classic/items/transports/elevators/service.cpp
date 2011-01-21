#include "service.h"

using namespace OSS;
using namespace Classic;


ItemDescriptor ServiceElevatorItem::descriptor = {
	kServiceElevatorType,
	kElevatorGroup,
	kTransportCategory,
	1,
	(0),
	100000,
	int2(4, 1)
};





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

ServiceElevatorItem::ServiceElevatorItem(Tower * tower) : ElevatorItem(tower, &descriptor)
{
}
