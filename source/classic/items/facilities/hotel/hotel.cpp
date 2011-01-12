#include "hotel.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

HotelItem::HotelItem(Tower * tower, ItemDescriptor * descriptor) : OccupiableItem(tower, descriptor)
{
	state = kEmptyState;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark State
//----------------------------------------------------------------------------------------------------

HotelItem::State HotelItem::getState() const
{
	return state;
}

void HotelItem::setState(State state)
{
	if (this->state != state) {
		this->state = state;
		onChangeState();
	}
}

void HotelItem::onChangeState()
{
	updateBackground();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Basic Sprites
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
	//Choose the base index
	unsigned int index;
	switch (getState()) {
		case kOccupiedState:	index = 0; break;
		case kAsleepState:		index = 2; break;
		case kEmptyState:		index = 3; break;
		case kDirtyState:		index = 5; break;
		case kInfestedState:	index = 7; break;
	}
	
	//If the index doesn't point at the asleep slice and the room is not lit, select the next slice
	//which is the unlit version.
	if (index != 2 && !isLit())
		index++;
	
	return index;
}

void HotelItem::updateBackground()
{
	FacilityItem::updateBackground();
	
	//Determine what slice of the textures we're supposed to show
	unsigned int slice = getTextureSliceIndex();
	
	//The first slice is in its own texture for some reason
	if (slice == 0) {
		backgrounds[0].texture = Engine::Texture::named(getTextureBaseName() + "/0");
		backgrounds[0].textureRect = rectd(0, 0, 1, 1);
	} else {
		backgrounds[0].texture = Engine::Texture::named(getTextureBaseName() + "/1");
		backgrounds[0].textureRect = rectd((slice - 1) * 0.125, 0, 0.125, 1);
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

void HotelItem::advance(double dt)
{
	OccupiableItem::advance(dt);
		
	//Update the guests
	updateGuests();
	
	//Everyone should be asleep at 1:30
	if (tower->checkTime(1.5))
		assert(areAllGuestsAsleep());
	
	//There shouldn't be any more guests after 12:00
	if (tower->checkTime(12))
		assert(guests.empty());
	
	//Animate the guests
	if (isOccupied() && getState() != kAsleepState)
		advanceGuests(dt);
	
	//Animate the janitor
	if (hasAssignedJanitor())
		getAssignedJanitor()->advanceAnimation(dt);
}

void HotelItem::advanceGuests(double dt)
{
	for (Guests::iterator i = guests.begin(); i != guests.end(); i++)
		(*i)->advanceAnimation(dt);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

void HotelItem::draw(rectd visibleRect)
{
	OccupiableItem::draw(visibleRect);
	
	//Draw our guests if required
	if (isOccupied() && getState() != kAsleepState)
		drawGuests(visibleRect);
	
	//Draw the janitor if required
	if (hasAssignedJanitor())
		getAssignedJanitor()->drawAnimation(visibleRect);
}

void HotelItem::drawGuests(rectd visibleRect)
{
	for (Guests::iterator g = guests.begin(); g != guests.end(); g++)
		(*g)->drawAnimation(visibleRect);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Guests
//----------------------------------------------------------------------------------------------------

unsigned int HotelItem::getMaxNumberOfGuests()
{
	return 2;
}

void HotelItem::initGuests()
{
	unsigned int numGuests = getMaxNumberOfGuests();
	for (int i = 0; i < numGuests; i++) {
		HotelGuest * guest = new HotelGuest(tower, this);
		guest->setType(i == 0 ? Person::kManType : Person::kWomanBType);
		guests.insert(guest);
	}
}

void HotelItem::updateGuests()
{
	Guests temp = guests;
	for (Guests::iterator g = temp.begin(); g != temp.end(); g++)
		(*g)->update();
}

void HotelItem::clearGuests()
{
	guests.clear();
}

void HotelItem::addPerson(Person * person)
{
	OccupiableItem::addPerson(person);
	
	//Special treatment for janitors
	if (person->isKindOfClass(typeid(Janitor))) {
		//Mark the hotel as clean and empty
		setState(kEmptyState);
	}
}

void HotelItem::removePerson(Person * person)
{
	OccupiableItem::removePerson(person);
	
	//Special treatment for hotel guests
	if (person->isKindOfClass(typeid(HotelGuest))) {
		//Check whether the guest is leaving
		if (((HotelGuest *)person)->isLeaving()) {
			guests.erase((HotelGuest *)person);
			
			//If there are no more guests, mark the hotel item as vacant
			if (guests.empty())
				setOccupied(false);
		}
	}
}

bool HotelItem::areAllGuestsAsleep()
{
	for (Guests::iterator g = guests.begin(); g != guests.end(); g++)
		if (!(*g)->isAsleep())
			return false;
	return true;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Housekeeping
//----------------------------------------------------------------------------------------------------

Janitor * HotelItem::getAssignedJanitor()
{
	return assignedJanitor;
}

void HotelItem::setAssignedJanitor(Janitor * janitor)
{
	if (assignedJanitor != janitor) {
		assignedJanitor = janitor;
		//backgrounds[0].color = (assignedJanitor ? (color4d){0.25, 1, 0.25, 1} : (color4d){1, 1, 1, 1});
	}
}

bool HotelItem::hasAssignedJanitor()
{
	return (getAssignedJanitor() != NULL);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Notifications
//----------------------------------------------------------------------------------------------------

void HotelItem::initAutooccupyTime()
{
	OSSObjectLog << std::endl;
	setAutooccupyTime(randd(std::max<double>(tower->time->getTime(), 17), 21));
}

void HotelItem::onChangeOccupied()
{
	OSSObjectLog << std::endl;
	if (isOccupied()) {
		setState(kOccupiedState);
		initGuests();
	} else {
		setState(kDirtyState);
		clearGuests();
		
		//Dispatch the event
		/*GameEvent event = {kGameEventHotelVacated};
		event.hotel.hotel = this;
		tower->handleEvent(&event);*/
	}
}

void HotelItem::onChangeGuestAsleep()
{
	OSSObjectLog << std::endl;
	setState(areAllGuestsAsleep() ? kAsleepState : kOccupiedState);
}
