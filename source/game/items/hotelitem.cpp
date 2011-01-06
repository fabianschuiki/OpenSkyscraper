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
	
	//Perform guest animation if required
	if (isOccupied() && getState() != kAsleepState) {
		guestAnimationTimer += dt;
		if (guestAnimationTimer >= 1) {
			guestAnimationTimer -= 1;
			updateGuestAnimation();
		}
	}
}

void HotelItem::updateGuestAnimation()
{
	for (Guests::iterator g = guests.begin(); g != guests.end(); g++) {
		//Calculate the cell position
		recti rect = getRect();
		rect.origin.x = randi(rect.minX(), rect.maxX() - 2);
		rect.size.x = 2;
		
		//Calculate the world position
		rectd worldRect = tower->convertCellToWorldRect(rect);
		worldRect.size.y = backgrounds[0].getRect().size.y;
		(*g)->sprite.setRect(worldRect);
		
		//Choose a new slice of the image
		(*g)->shuffleSprite();
	}
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
}

void HotelItem::drawGuests(rectd visibleRect)
{
	for (Guests::iterator g = guests.begin(); g != guests.end(); g++)
		if ((*g)->isAt(this))
			(*g)->sprite.draw(visibleRect);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Guests
//----------------------------------------------------------------------------------------------------

void HotelItem::initGuests()
{
	guests.insert(new HotelGuest(tower, this));
	updateGuestAnimation();
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

void HotelItem::removePerson(Person * person)
{
	OccupiableItem::removePerson(person);
	
	if (((HotelGuest *)person)->isLeaving()) {
		OSSObjectLog << std::endl;
		guests.erase((HotelGuest *)person);
		
		//If there are no more guests, mark the hotel item as vacant
		if (guests.empty())
			setOccupied(false);
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
