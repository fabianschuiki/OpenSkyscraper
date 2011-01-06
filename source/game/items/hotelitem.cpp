#include "hotelitem.h"

using namespace OSS;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

HotelItem::HotelItem(Tower * tower, Item::Descriptor * descriptor) : OccupiableItem(tower, descriptor)
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

std::string HotelItem::getTextureBaseName()
{
	std::stringstream s;
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
		backgrounds[0].texture = Texture::named(getTextureBaseName() + "/0");
		backgrounds[0].textureRect = rectd(0, 0, 1, 1);
	} else {
		backgrounds[0].texture = Texture::named(getTextureBaseName() + "/1");
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
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Guests
//----------------------------------------------------------------------------------------------------

void HotelItem::initGuests()
{
	guests.insert(new HotelGuest(tower, this));
}

void HotelItem::updateGuests()
{
	Guests tempGuests = guests;
	for (Guests::iterator g = tempGuests.begin(); g != tempGuests.end(); g++)
		(*g)->update();
}

void HotelItem::clearGuests()
{
	guests.clear();
}

void HotelItem::removeGuest(HotelGuest * guest)
{
	OSSObjectLog << std::endl;
	guests.erase(guest);
	
	//If there are no more guests, mark the hotel item as vacant
	if (guests.empty())
		setOccupied(false);
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
#pragma mark Notifications
//----------------------------------------------------------------------------------------------------

void HotelItem::initAutooccupyTime()
{
	OSSObjectLog << std::endl;
	setAutooccupyTime(randd(std::max<double>(tower->time, 17), 21));
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
	}
}

void HotelItem::onChangeGuestAsleep()
{
	OSSObjectLog << std::endl;
	setState(areAllGuestsAsleep() ? kAsleepState : kOccupiedState);
}
