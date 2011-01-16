#include "hotelguest.h"
#include "../../items/facilities/hotel/hotel.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

HotelGuest::HotelGuest(Tower * tower, HotelItem * hotel) : Person(tower), hotel(hotel)
{
	initialVisitDone = false;
	hadDinner = false;
	sleepTime = 0;
	didSleep = false;
	didChooseSleepTime = false;
	asleep = false;
	
	//initAnimationSprite();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Intelligence
//----------------------------------------------------------------------------------------------------

/*HotelGuest::Intention HotelGuest::getIntention()
{
	return intention;
}

void HotelGuest::setIntention(Intention intention)
{
	if (this->intention != intention) {
		this->intention = intention;
	}
}*/

void HotelGuest::think()
{
	Person::think();
	OSSObjectLog << std::endl;
	
	//If we haven't slept yet
	if (!didSleep) {
		//In case we're awake...
		if (!isAsleep()) {
			
			//Do the initial visit to the room
			if (!initialVisitDone) {
				if (!isAt(hotel)) {
					OSSObjectLog << "doing initial visit" << std::endl;
					setDestination(hotel);
					return;
				} else {
					initialVisitDone = true;
					setPauseDuration(0.5);
					return;
				}
			}
			
			//Have dinner if it is before 21:00
			if (!hadDinner) {
				if (tower->time->getTimeOfDay() >= 18.5 && tower->time->getTimeOfDay() < 21 && isAt(hotel)) {
					OSSObjectLog << "going to have dinner" << std::endl;
					setDestination(NULL);
					return;
				} else {
					hadDinner = true;
					setPauseDuration(0.75);
					return;
				}
			}
			
			//Go back to the room
			if (!isAt(hotel)) {
				OSSObjectLog << "heading back to the hotel" << std::endl;
				setDestination(hotel);
				return;
			}
			
			//Decide when to go to sleep
			if (!didChooseSleepTime) {
				//Calculate the sleep time
				sleepTime = tower->time->getLogicalTodayRandom(23, 25.5);
				OSSObjectLog << "decided to go to sleep at " << sleepTime << std::endl;
				setPauseEndTime(sleepTime);
				didChooseSleepTime = true;
				return;
			}
			
			//Decide when to wake up and then go to sleep
			if (!didSleep) {
				setPauseEndTimeTomorrow(randd(6, 8));
				OSSObjectLog << "going to sleep, decided to wake up at " << getPauseEndTime() << std::endl;
				setAsleep(true);
				return;
			}
		}
		
		//Otherwise we wake up
		else {
			didSleep = true;
			setPauseEndTimeToday(7);
			OSSObjectLog << "waking up..." << std::endl;
			setAsleep(false);
			return;
		}
	}
	
	//Decide when to check out
	if (!checkingOut) {
		setPauseEndTimeToday(randd(tower->time->getLogicalTimeOfDay() + 0.25, 8));
		OSSObjectLog << "decided to check out at " << getPauseEndTime() << std::endl;
		checkingOut = true;
		return;
	}
	
	//Check out
	setDestination(NULL);
	OSSObjectLog << "checking out" << std::endl;
	//assert(getNextDestinationTime() <= 12);
}

bool HotelGuest::isCheckingOut()
{
	return checkingOut;
}

bool HotelGuest::isAsleep()
{
	return asleep;
}

void HotelGuest::setAsleep(bool asleep)
{
	if (this->asleep != asleep) {
		this->asleep = asleep;
		//hotel->onChangeGuestAsleep();
		hotel->updateBackgroundIfNeeded.setNeeded();
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Animation Sprite
//----------------------------------------------------------------------------------------------------

/*void HotelGuest::initAnimationSprite()
{	
	//Load the texture and set the slice size
	animationSprite.texture = Texture::named("simtower/facilities/hotel/guests");
	animationSprite.rect = rectd(0, 0, 16, 24);
	animationSprite.textureRect.size.x = (1.0 / 16);
	
	//Do the rest of the initialization
	TimedPerson::initAnimationSprite();
}

void HotelGuest::updateAnimationSprite()
{
	TimedPerson::updateAnimationSprite();
	
	//Set the texture rect
	animationSprite.textureRect.origin.x = getAnimationIndex() / 16.0;
}

bool HotelGuest::shouldAnimate()
{
	return isAt(hotel);
}

void HotelGuest::shuffleAnimation()
{
	TimedPerson::shuffleAnimation();
	
	//Set the animation index
	setAnimationIndex(getGender() == kMale ? randi(0, 10) : randi(11, 15));
	
	//Position the guest
	setAnimationLocation(int2(randi(0, hotel->getRect().size.x - 2), 0));
}*/





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark State
//----------------------------------------------------------------------------------------------------


