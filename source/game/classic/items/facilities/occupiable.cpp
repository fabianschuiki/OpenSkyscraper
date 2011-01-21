#include "occupiable.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

OccupiableItem::OccupiableItem(Tower * tower, ItemDescriptor * descriptor) :
FacilityItem(tower, descriptor),
updateOccupyAtIfNeeded(this, &OccupiableItem::updateOccupyAt, &updateIfNeeded)
{
	occupied = false;
	occupyAt = 0;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Occupation
//----------------------------------------------------------------------------------------------------

bool OccupiableItem::isOccupied() const
{
	return occupied;
}

void OccupiableItem::setOccupancy(bool o)
{
	if (this->occupied != o) {
		willChangeOccupancy(o);
		
		//Set the occupyAt time to 0 so that it gets updated later if we're vacating the item.
		if (!o)
			occupyAt = 0;
		
		//Apply the new state
		this->occupied = o;
		
		didChangeOccupancy();
		updateBackgroundIfNeeded.setNeeded();
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

double OccupiableItem::getOccupyAt()
{
	return occupyAt;
}

void OccupiableItem::setOccupyAt(double oa)
{
	if (occupyAt != oa) {
		occupyAt = oa;
		OSSObjectLog << getTypeName() << " will be occupied at " << oa << std::endl;
	}
}



void OccupiableItem::advanceItem(double dt)
{
	FacilityItem::advanceItem(dt);
	
	//Advance the occupancy
	advanceOccupancy(dt);
}

void OccupiableItem::advanceOccupancy(double dt)
{
	//If we aren't occupied we have to deal with people moving in.
	if (!isOccupied()) {
		
		//Only deal with this if the item is attractive enough for people to move in.
		if (isSufficientlyAttractive()) {
			
			//If the occupyAt time is invalid, mark it as to be updated.
			if (occupyAt <= 0)
				updateOccupyAtIfNeeded.setNeeded();
			
			//If the occupyAt time has been reached, make people move in.
			else if (tower->time->getTime() >= occupyAt)
				setOccupancy(true);
		}
	}
	
	//Otherwise we have to deal with people possibly moving out.
	else {
		
		//Only deal with this if the item is not attractive enough for people to stay.
		if (!isSufficientlyAttractive() && tower->time->checkDaily(5))
			setOccupancy(false);
	}
}

bool OccupiableItem::isSufficientlyAttractive()
{
	return true;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark State
//----------------------------------------------------------------------------------------------------

void OccupiableItem::update()
{
	FacilityItem::update();
	
	//Update the occupyAt time if required
	updateOccupyAtIfNeeded();
}
