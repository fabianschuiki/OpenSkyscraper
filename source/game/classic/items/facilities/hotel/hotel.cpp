#include "hotel.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

HotelItem::HotelItem(Tower * tower, ItemDescriptor * descriptor) :
OccupiableItem(tower, descriptor)
{
	asleep = false;
	dirty = false;
	infested = false;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Guests
//----------------------------------------------------------------------------------------------------

unsigned int HotelItem::getMaxNumberOfGuests()
{
	return 2;
}



bool HotelItem::areAllGuestsAsleep()
{
	for (Guests::iterator it = guests.begin(); it != guests.end(); it++)
		if (!(*it)->isAsleep())
			return false;
	return true;
}

bool HotelItem::haveAllGuestsCheckedOut()
{
	for (Guests::iterator it = guests.begin(); it != guests.end(); it++)
		if (!(*it)->isCheckingOut())
			return false;
	return true;
}



void HotelItem::didChangeOccupancy()
{
	//If the hotel room has just been occupied, initialize the missing guests
	if (isOccupied()) {
		unsigned int numberOfGuests = getMaxNumberOfGuests();
		unsigned int index = 0;
		
		while (guests.size() < numberOfGuests) {
			HotelGuest * guest = new HotelGuest(tower, this);
			//guest->updateIfNeeded.parent = &updateItemIfNeeded;
			guest->setType(index++ == 0 ? Person::kManType : Person::kWomanBType);
			guests.insert(guest);
		}
	}
	
	//If the hotel room has just been vacated, mark it as dirty and remove all guests.
	else {
		guests.clear();
		setDirty(true);
		
		//Send an event informing all the other items that the hotel has just been vacated.
		tower->sendEvent(new ItemEvent<HotelItem>(Event::kHotelVacated, this));
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Janitor
//----------------------------------------------------------------------------------------------------

Janitor * HotelItem::getAssignedJanitor()
{
	return assignedJanitor;
}

void HotelItem::setAssignedJanitor(Janitor * janitor)
{
	if (assignedJanitor != janitor) {
		assert(assignedJanitor == NULL || janitor == NULL);
		assignedJanitor = janitor;
	}
}

bool HotelItem::hasAssignedJanitor()
{
	return (getAssignedJanitor() != NULL);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark People
//----------------------------------------------------------------------------------------------------

void HotelItem::didAddPerson(Person * person)
{	
	FacilityItem::didAddPerson(person);
	
	//If the janitor just arrived, mark the room as cleaned up.
	if (person->getTypeName() == "hotel/janitor")
		setDirty(false);
}

void HotelItem::willRemovePerson(Person * person)
{
	FacilityItem::willRemovePerson(person);
	
	//If the person that is leaving the hotel room is a guest and the guest has decided to check
	//out, we have to remove him from the list of guests.
	if (person->getTypeName() == "hotel/guest" && ((HotelGuest *)person)->isCheckingOut()) {
		
		//Remove the person from the guest set.
		//TODO: remove this, we have another way of telling
		//guests.erase((HotelGuest *)person);
		
		//If all guests have checked out, mark the hotel room as vacant. This should call the next
		//available janitor to the room for cleaning.
		if (haveAllGuestsCheckedOut())
			setOccupancy(false);
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

void HotelItem::advanceItem(double dt)
{
	OccupiableItem::advanceItem(dt);
	
	//Everyone should be asleep at 1:30
	/*if (tower->time->checkDaily(1.5))
		assert(areAllGuestsAsleep());*/
	
	//There shouldn't be any more guests after 12:00
	/*if (tower->time->checkDaily(12))
		assert(guests.empty());*/
	
	backgrounds[0]->color = (hasAssignedJanitor() ? (color4d){0.5, 1, 0.5, 1.0} : (color4d){1, 1, 1, 1});
	
	//Simulate the guests
	/*if (isOccupied())
		for (Guests::iterator it = guests.begin(); it != guests.end(); it++)
			(*it)->advance(dt);*/
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark State
//----------------------------------------------------------------------------------------------------

bool HotelItem::isAsleep()
{
	return asleep;
}

void HotelItem::setAsleep(bool a)
{
	if (asleep != a) {
		asleep = a;
		updateBackgroundIfNeeded.setNeeded();
	}
}

bool HotelItem::isDirty()
{
	return dirty;
}

void HotelItem::setDirty(bool d)
{
	if (dirty != d) {
		dirty = d;
		updateBackgroundIfNeeded.setNeeded();
	}
}

bool HotelItem::isInfested()
{
	return infested;
}

void HotelItem::setInfested(bool i)
{
	if (infested != i) {
		infested = i;
		updateBackgroundIfNeeded.setNeeded();
	}
}

void HotelItem::updateItem()
{
	OccupiableItem::updateItem();
	
	//Set the asleep state according to whether all guests have gone to sleep
	setAsleep(areAllGuestsAsleep());
}

void HotelItem::updateBackground()
{
	OccupiableItem::updateBackground();
	
	//Determine what slice of the textures we're supposed to show
	unsigned int slice = getTextureSliceIndex();
	
	//Load the appropriate background textures. The first slice is in its own texture for some
	//reason.
	if (slice == 0) {
		backgrounds[0]->texture = Texture::named(getTextureBaseName() + "/0");
		backgrounds[0]->textureRect = rectd(0, 0, 1, 1);
	} else {
		backgrounds[0]->texture = Texture::named(getTextureBaseName() + "/1");
		backgrounds[0]->textureRect = rectd((slice - 1) * 0.125, 0, 0.125, 1);
	}
}

void HotelItem::updateOccupyAt()
{
	setOccupyAt(tower->time->getLogicalTodayRandom(17, 21));
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

string HotelItem::getTextureBaseName()
{
	stringstream s;
	s << "simtower/facilities/hotel/";
	s << getTypeName();
	s << "/";
	s << getVariant();
	return s.str();
}

unsigned int HotelItem::getTextureSliceIndex()
{
	//Choose which slice index should be drawn based on the current hotel room state.
	unsigned int index;
	if (isOccupied()) {
		if (isAsleep())
			index = 2;
		else
			index = 0;
	} else {
		if (isInfested())
			index = 7;
		else if (isDirty())
			index = 5;
		else
			index = 3;
	}
	
	//If the index doesn't point at the asleep slice and the room is not lit, select the next slice
	//which is the unlit version.
	if (index != 2 && !isLit())
		index++;
	
	return index;
}

