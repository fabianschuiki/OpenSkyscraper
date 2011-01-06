#include "janitor.h"
#include "../items/housekeeping.h"
#include "../items/hotelitem.h"

using namespace OSS;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

Janitor::Janitor(Tower * tower, HousekeepingItem * housekeeping)
: TimedPerson(tower), housekeeping(housekeeping)
{
	setType(kJanitorType);
	setItem(housekeeping);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Hotel
//----------------------------------------------------------------------------------------------------

HotelItem * Janitor::getAssignedHotel()
{
	return assignedHotel;
}

void Janitor::setAssignedHotel(HotelItem * hotel)
{
	if (assignedHotel != hotel) {
		if (assignedHotel)
			assignedHotel->setAssignedJanitor(NULL);
		assignedHotel = hotel;
		if (assignedHotel)
			assignedHotel->setAssignedJanitor(this);
		onChangeAssignedHotel();
	}
}

bool Janitor::hasAssignedHotel()
{
	return (getAssignedHotel() != NULL);
}

void Janitor::onChangeAssignedHotel()
{
	//If we have an assigned hotel, move there
	if (getAssignedHotel()) {
		OSSObjectLog << "assigned to hotel " << getAssignedHotel()->description() << std::endl;
		setNextDestination(getAssignedHotel(), 0.25);
	}
	
	//Otherwise go back to housekeeping
	else {
		OSSObjectLog << "going back to " << housekeeping->description() << std::endl;
		setNextDestination(housekeeping);
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Intelligence
//----------------------------------------------------------------------------------------------------

void Janitor::think()
{
	//Idle when at the housekeeping facility
	if (isAt(housekeeping))
		return;
	
	//Otherwise move on
	assert(getAssignedHotel());
	housekeeping->onJanitorDone(this);
}
