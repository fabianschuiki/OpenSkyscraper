#include "occupiable.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

OccupiableItem::OccupiableItem(Tower * tower, ItemDescriptor * descriptor) : FacilityItem(tower, descriptor)
{
	occupied = false;
	autooccupyTime = 25;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Occupation
//----------------------------------------------------------------------------------------------------

bool OccupiableItem::isOccupied() const
{
	return occupied;
}

void OccupiableItem::setOccupied(bool occupied)
{
	if (this->occupied != occupied) {
		this->occupied = occupied;
		OSSObjectLog << "occupied = " << occupied << std::endl;
		if (!occupied)
			resetOccupancyAutomation();
		onChangeOccupied();
	}
}

void OccupiableItem::onChangeOccupied()
{
	updateBackground();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Occupancy Automation
//----------------------------------------------------------------------------------------------------

double OccupiableItem::getAutooccupyTime()
{
	return autooccupyTime;
}

void OccupiableItem::setAutooccupyTime(double time)
{
	if (autooccupyTime != time) {
		autooccupyTime = time;
		OSSObjectLog << time << std::endl;
		updateOccupancyAutomation();
	}
}

void OccupiableItem::initAutooccupyTime()
{
	setAutooccupyTime(24);
}

void OccupiableItem::resetOccupancyAutomation()
{
	setAutooccupyTime(25);
}

void OccupiableItem::updateOccupancyAutomation()
{
	if (tower->time >= getAutooccupyTime())
		setOccupied(true);
}

bool OccupiableItem::isSufficientlyAttractive()
{
	return true;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

void OccupiableItem::advance(double dt)
{
	FacilityItem::advance(dt);
	
	if (!isOccupied() && isSufficientlyAttractive()) {
		if (tower->checkTime(5) || getAutooccupyTime() > 24)
			initAutooccupyTime();
		updateOccupancyAutomation();
	}
}
