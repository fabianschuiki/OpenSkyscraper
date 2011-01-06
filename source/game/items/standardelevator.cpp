#include "standardelevator.h"

using namespace OSS;


Item::Descriptor StandardElevatorItem::descriptor = {
	Item::kStandardElevatorType,
	Item::kElevatorGroup,
	Item::kTransportCategory,
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
