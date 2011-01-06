#include "housekeeping.h"
#include "hotelitem.h"

using namespace OSS;


Item::Descriptor HousekeepingItem::descriptor = {
	Item::kHousekeepingType,
	Item::kServicesGroup,
	Item::kFacilityCategory,
	1,
	(Item::kUndestructibleAttribute),
	50000,
	int2(15, 1)
};





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

HousekeepingItem::HousekeepingItem(Tower * tower) : FacilityItem(tower, &descriptor)
{
}

void HousekeepingItem::init()
{
	FacilityItem::init();
	initJanitors();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Basic Sprites
//----------------------------------------------------------------------------------------------------

void HousekeepingItem::initBackground()
{
	FacilityItem::initBackground();
	backgrounds[0].texture = Texture::named("simtower/facilities/housekeeping");
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

void HousekeepingItem::advance(double dt)
{
	FacilityItem::advance(dt);
	updateJanitors();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Janitors
//----------------------------------------------------------------------------------------------------

void HousekeepingItem::initJanitors()
{
	janitors.clear();
	for (int i = 0; i < 6; i++) {
		Janitor * janitor = new Janitor(tower, this);
		janitors.insert(janitor);
	}
}

void HousekeepingItem::updateJanitors()
{
	for (JanitorSet::iterator i = janitors.begin(); i != janitors.end(); i++)
		(*i)->update();
}

void HousekeepingItem::onJanitorDone(Janitor * janitor)
{
	if (!janitor) return;
	
	OSSObjectLog << "janitor is done" << std::endl;
	
	//Find the leftmost hotel on the closest floor that is dirty
	HotelItem * nextHotel = NULL;
	for (Tower::ItemIDMap::iterator i = tower->items.begin(); i != tower->items.end(); i++) {
		
		//Skip non-hotel items
		if (i->second->getGroup() != Item::kHotelGroup)
			continue;
		HotelItem * hotel = (HotelItem *)((Item *)i->second);
		
		//Check whether the hotel is dirty and doesn't have a janitor yet
		if (hotel->getState() == HotelItem::kDirtyState && !hotel->hasAssignedJanitor()) {
			if (nextHotel) {				
				//Calculate the distance between the items
				int distance = abs(hotel->getMinFloor() - janitor->getFloor());
				int nextDistance = abs(nextHotel->getMinFloor() - janitor->getFloor());
				
				//Check whether the hotel is vertically closer than the old one
				if (nextDistance < distance) {
					/*OSSObjectLog << "discarding on " << hotel->getMinFloor() << ", " << nextDistance
					<< " > " << distance << std::endl;*/
					continue;
				}
				
				//Check whether the hotel is more to the left than the one we have
				if (nextHotel->getRect().origin.x < hotel->getRect().origin.x) {
					/*OSSObjectLog << "discarding on " << hotel->getMinFloor() << ", " << nextDistance
					<< " > " << distance << std::endl;*/
					continue;
				}
			}
			
			//Obviously nothing disqualified the hotel, so take it
			nextHotel = hotel;
		}
	}
	
	//Assign the hotel
	janitor->setAssignedHotel(nextHotel);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Events
//----------------------------------------------------------------------------------------------------

void HousekeepingItem::eventHotelVacated(HotelItem * hotel)
{
	OSSObjectLog << hotel->description() << std::endl;
	
	//Find a janitor to assign to this hotel if it doesn't yet have a janitor assigned
	if (!hotel->hasAssignedJanitor()) {
		for (JanitorSet::iterator i = janitors.begin(); i != janitors.end(); i++) {
			if ((*i)->isAt(this)) {
				(*i)->setAssignedHotel(hotel);
				break;
			}
		}
	}
}
