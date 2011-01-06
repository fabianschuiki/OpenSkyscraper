#include "hotelguest.h"
#include "hotelitem.h"
#include "tower.h"

using namespace OSS;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

HotelGuest::HotelGuest(Tower * tower, HotelItem * hotel) : TimedPerson(tower), hotel(hotel)
{
	initialVisitDone = false;
	hadDinner = false;
	sleepTime = 0;
	didSleep = false;
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
	TimedPerson::think();
	OSSObjectLog << std::endl;
	
	//Do the initial visit to the room
	if (!initialVisitDone) {
		if (!isAt(hotel)) {
			OSSObjectLog << "doing initial visit" << std::endl;
			setNextDestination(hotel, 0.5);
			return;
		} else
			initialVisitDone = true;
	}
	
	//Have dinner if it is before 21:00
	if (!hadDinner) {
		if (tower->time < 21 && isAt(hotel)) {
			OSSObjectLog << "going to have dinner" << std::endl;
			setNextDestination(NULL, 0.75);
			return;
		}
		hadDinner = true;
	}
	
	//Go back to the room
	if (!isAt(hotel)) {
		OSSObjectLog << "heading back to the hotel" << std::endl;
		setNextDestination(hotel);
		return;
	}
	
	//Decide when to go to sleep
	if (sleepTime < 23) {
		sleepTime = randd(std::max<double>(tower->time, 23), 25.5);
		OSSObjectLog << "decided to go to sleep at " << sleepTime << std::endl;
		setPauseEndTime(sleepTime);
		return;
	}
	
	//Decide when to wake up and go to sleep
	if (!didSleep && !isAsleep()) {
		setPauseEndTimeFuture(randd(7, 8));
		OSSObjectLog << "going to sleep, decided to wake up at " << getPauseEndTime() << std::endl;
		setAsleep(true);
		return;
	}
	
	//Wake up and decide when to leave
	if (isAsleep()) {
		didSleep = true;
		setPauseDuration(randd(0.5, 1.5));
		OSSObjectLog << "waking up. going to leave at " << getPauseEndTime() << std::endl;
		setAsleep(false);
		return;
	}
	
	//Leave
	OSSObjectLog << "leaving" << std::endl;
	setNextDestination(NULL);
	hotel->removeGuest(this);
}

bool HotelGuest::isAsleep()
{
	return asleep;
}

void HotelGuest::setAsleep(bool asleep)
{
	if (this->asleep != asleep) {
		this->asleep = asleep;
		hotel->onChangeGuestAsleep();
	}
}
