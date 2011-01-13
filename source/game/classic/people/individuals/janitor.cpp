#include "janitor.h"
#include "../../items/facilities/hotel/housekeeping.h"
#include "../../items/facilities/hotel/hotel.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

Janitor::Janitor(Tower * tower, HousekeepingItem * housekeeping)
: TimedPerson(tower), housekeeping(housekeeping)
{
	initAnimationSprite();
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
	if (getAssignedHotel())
		setNextDestination(getAssignedHotel(), 0.25);
	
	//Otherwise go back to housekeeping
	else
		setNextDestination(housekeeping);
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





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Animation Sprite
//----------------------------------------------------------------------------------------------------

void Janitor::initAnimationSprite()
{	
	//Load the texture and set the slice size
	animationSprite.texture = Texture::named("simtower/facilities/hotel/janitor");
	animationSprite.rect = rectd(0, 0, 16, 24);
	animationSprite.textureRect.size.x = (1.0 / 3);
	
	//Do the rest of the initialization
	TimedPerson::initAnimationSprite();
}

void Janitor::updateAnimationSprite()
{
	TimedPerson::updateAnimationSprite();
	
	//Set the texture rect
	animationSprite.textureRect.origin.x = getAnimationIndex() / 3.0;
}

bool Janitor::shouldAnimate()
{
	return (getItem() && getItem()->getGroup() == kHotelGroup);
}

void Janitor::shuffleAnimation()
{
	TimedPerson::shuffleAnimation();
	if (hasAssignedHotel()) {
		setAnimationIndex(randi(0, 2));
		setAnimationLocation(int2(randi(0, getAssignedHotel()->getRect().size.x - 2), 0));
	}
}
