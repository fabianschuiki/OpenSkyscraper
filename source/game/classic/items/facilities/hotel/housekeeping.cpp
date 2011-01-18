#include "housekeeping.h"

#include "hotel.h"

using namespace OSS;
using namespace Classic;


ItemDescriptor HousekeepingItem::descriptor = {
	kHousekeepingType,
	kHotelGroup,
	kFacilityCategory,
	1,
	(kUndestructibleAttribute),
	50000,
	int2(15, 1)
};





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

HousekeepingItem::HousekeepingItem(Tower * tower) : FacilityItem(tower, &descriptor)
{
	//Create the janitors that work in this housekeeping facility
	for (int i = 0; i < 6; i++) {
		Janitor * janitor = new Janitor(tower, this);
		janitors.insert(janitor);
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Janitors
//----------------------------------------------------------------------------------------------------

void HousekeepingItem::onJanitorDone(Janitor * janitor)
{
	if (!janitor) return;
	
	OSSObjectLog << "janitor " << janitor->description() << " is done" << std::endl;
	
	//Find the leftmost hotel on the closest floor that is dirty
	HotelItem * nextHotel = NULL;
	TowerStructure::ItemSet hotels = tower->structure->getItems(kHotelGroup);
	for (TowerStructure::ItemSet::iterator i = hotels.begin(); i != hotels.end(); i++) {
		
		//Skip housekeepers
		if ((*i)->getType() == kHousekeepingType)
			continue;
		HotelItem * hotel = (HotelItem *)((Item *)*i);
		
		//Check whether the hotel is dirty and doesn't have a janitor yet
		if (hotel->isDirty() && !hotel->hasAssignedJanitor()) {
			if (nextHotel) {				
				//Calculate the distance between the items
				int distance = abs(hotel->getMinFloor() - janitor->getFloor());
				int nextDistance = abs(nextHotel->getMinFloor() - janitor->getFloor());
				
				//Check whether the hotel is vertically closer than the old one
				if (nextDistance < distance)
					continue;
				
				//Check whether the hotel is more to the left than the one we have
				if (nextHotel->getRect().origin.x < hotel->getRect().origin.x)
					continue;
			}
			
			//Obviously nothing disqualified the hotel, so take it
			nextHotel = hotel;
		}
	}
	
	//Assign the hotel
	janitor->setAssignedHotel(nextHotel);
}

void HousekeepingItem::didChangeRect()
{
	FacilityItem::didChangeRect();
	
	//Make sure the janitors move to the new location
	for (JanitorSet::iterator i = janitors.begin(); i != janitors.end(); i++)
		if ((*i)->isAt(this))
			(*i)->setFloor(getRect().minY());
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark State
//----------------------------------------------------------------------------------------------------

void HousekeepingItem::updateBackground()
{
	FacilityItem::updateBackground();
	
	//Load the appropriate texture
	backgrounds[0]->texture = Texture::named("simtower/facilities/housekeeping");
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Events
//----------------------------------------------------------------------------------------------------

void HousekeepingItem::eventHotelVacated(ItemEvent<HotelItem> * event)
{
	OSSObjectLog << event->item->description() << std::endl;
	
	//Find a janitor to assign to this hotel if it doesn't yet have a janitor assigned
	if (!event->item->hasAssignedJanitor()) {
		for (JanitorSet::iterator i = janitors.begin(); i != janitors.end(); i++) {
			if ((*i)->isAt(this) && !(*i)->hasAssignedHotel()) {
				(*i)->setAssignedHotel(event->item);
				break;
			}
		}
	}
}
