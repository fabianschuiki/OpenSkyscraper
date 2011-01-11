#include "standardelevator.h"

using namespace OSS;
using namespace Classic;


ItemDescriptor StandardElevatorItem::descriptor = {
	kStandardElevatorType,
	kElevatorGroup,
	kTransportCategory,
	1,
	(0),
	200000,
	int2(4, 1)
};





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

StandardElevatorItem::StandardElevatorItem(Tower * tower) : ElevatorItem(tower, &descriptor)
{
}
