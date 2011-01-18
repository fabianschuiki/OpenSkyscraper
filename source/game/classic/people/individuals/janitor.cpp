#include "janitor.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

Janitor::Janitor(Tower * tower, HousekeepingItem * housekeeping)
: Person(tower), housekeeping(housekeeping)
{
	cleaningDone = false;
	setType(kJanitorType);
	setItem(housekeeping);
	setDestination(housekeeping);
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
		willChangeAssignedHotel();
		assignedHotel = hotel;
		didChangeAssignedHotel();
	}
}

bool Janitor::hasAssignedHotel()
{
	return (getAssignedHotel() != NULL);
}

void Janitor::willChangeAssignedHotel()
{
	if (hasAssignedHotel())
		getAssignedHotel()->setAssignedJanitor(NULL);
}

void Janitor::didChangeAssignedHotel()
{
	if (hasAssignedHotel())
		getAssignedHotel()->setAssignedJanitor(this);
	
	//We're not done cleaning the new hotel
	cleaningDone = false;
	
	//If we have an assigned hotel, move there
	if (hasAssignedHotel())
		setDestination(getAssignedHotel());
	
	//Otherwise go back to housekeeping
	else
		setDestination(housekeeping);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Animation
//----------------------------------------------------------------------------------------------------

void Janitor::updateAnimation()
{
	Person::updateAnimation();
	if (!shouldAnimate())
		return;
	
	//Create the animation sprite if required
	if (!animationSprite) {
		animationSprite = new Sprite;
		
		//Load the texture and set the slice size
		animationSprite->texture = Texture::named("simtower/facilities/hotel/janitor");
		animationSprite->rect = rectd(0, 0, 16, 24);
		animationSprite->textureRect.size.x = (1.0 / 3);
	}
	
	//Position the housekeeper
	int2 position = getAssignedHotel()->getRect().origin;
	position.x += randi(0, getAssignedHotel()->getRect().size.x - 2);
	animationSprite->rect.origin = tower->structure->cellToWorld(position);
	
	//Set the texture rect
	animationSprite->textureRect.origin.x = randi(0, 2) / 3.0;
}

bool Janitor::shouldAnimate()
{
	return (hasAssignedHotel() && isAt(getAssignedHotel()));
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

void Janitor::think()
{
	OSSObjectLog << std::endl;
	
	//Idle when at the housekeeping facility
	if (isAt(housekeeping)) {
		setPauseDuration(24);
		return;
	}
	
	//If we haven't cleaned the hotel, we have to do so first.
	assert(getAssignedHotel());
	if (!cleaningDone) {
		setPauseDuration(0.25);
		cleaningDone = true;
		return;
	}
	
	//We're done with everything, move on.
	housekeeping->onJanitorDone(this);
}
