#include "entertainment.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

EntertainmentItem::EntertainmentItem(Tower * tower, ItemDescriptor * descriptor) :
FacilityItem(tower, descriptor)
{
	open = false;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Openness
//----------------------------------------------------------------------------------------------------

bool EntertainmentItem::isOpen() const
{
	return open;
}

void EntertainmentItem::setOpen(bool o)
{
	if (open != o) {
		willChangeOpenness(o);
		open = o;
		didChangeOpenness();
		updateBackgroundIfNeeded.setNeeded();
		OSSObjectLog << (isOpen() ? "opening" : "closing") << std::endl;
	}
}

bool EntertainmentItem::shouldBeOpen()
{
	return false;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

void EntertainmentItem::advanceItem(double dt)
{
	FacilityItem::advanceItem(dt);
	
	//Open or close the item if appropriate.
	advanceOpenness(dt);
}

void EntertainmentItem::advanceOpenness(double dt)
{
	setOpen(shouldBeOpen());
}

