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
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Animation
//----------------------------------------------------------------------------------------------------

void HotelGuest::updateAnimation()
{
	Person::updateAnimation();
	if (!shouldAnimate())
		return;
	
	//Initialize the animation sprite if required
	if (!animationSprite) {
		animationSprite = new Sprite;
		
		//Load the texture and set the slice size
		animationSprite->texture = Texture::named("simtower/facilities/hotel/guests");
		animationSprite->rect = rectd(0, 0, 16, 24);
		animationSprite->textureRect.size.x = (1.0 / 16);
	}
	
	//Set the animation index
	unsigned int animationIndex = (getGender() == kMale ? randi(0, 10) : randi(11, 15));
	
	//Position the guest
	int2 position = hotel->getRect().origin;
	position.x += randi(0, hotel->getRect().size.x - 2);
	animationSprite->rect.origin = tower->structure->cellToWorld(position);
	
	//Set the texture rect
	animationSprite->textureRect.origin.x = animationIndex / 16.0;
}

bool HotelGuest::shouldAnimate()
{
	return isAt(hotel) && !asleep;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

bool HotelGuest::isAsleep()
{
	return asleep;
}

void HotelGuest::setAsleep(bool asleep)
{
	if (this->asleep != asleep) {
		this->asleep = asleep;
		hotel->updateItemIfNeeded.setNeeded();
	}
}

bool HotelGuest::isCheckingOut()
{
	return checkingOut;
}

void HotelGuest::think()
{
	Person::think();
	//OSSObjectLog << std::endl;
	
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
	if (!didChooseCheckoutTime) {
		setPauseEndTimeToday(randd(tower->time->getLogicalTimeOfDay() + 0.25, 11));
		OSSObjectLog << "decided to check out at " << getPauseEndTime() << std::endl;
		didChooseCheckoutTime = true;
		return;
	}
	
	//Check out
	if (!isAt(NULL)) {
		setDestination(NULL);
		OSSObjectLog << "checking out" << std::endl;
		checkingOut = true;
	}
	
	setPauseEndTimeToday(12);
}
