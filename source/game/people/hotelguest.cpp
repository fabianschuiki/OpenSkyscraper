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
	didChooseSleepTime = false;
	asleep = false;
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
	
	//In case we're awake...
	if (!isAsleep()) {
		
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
		if (!didChooseSleepTime) {
			//In case it is already the next day, we have to subtract 24h from our sleep times
			double offset = (tower->time < 6 ? 24 : 0);
			
			//Calculate the sleep time
			sleepTime = randd(std::max<double>(tower->time, 23 - offset), 25.5 - offset);
			OSSObjectLog << "decided to go to sleep at " << sleepTime << std::endl;
			setPauseEndTime(sleepTime);
			didChooseSleepTime = true;
			return;
		}
		
		//Decide when to wake up and go to sleep
		if (!didSleep) {
			setPauseEndTimeFuture(randd(6, 7));
			OSSObjectLog << "going to sleep, decided to wake up at " << getPauseEndTime() << std::endl;
			setAsleep(true);
			return;
		}
		
		/*//Decide when to leave
		if (didSleep) {
			setPauseDuration(randd(0.25, 0.75));
			OSSObjectLog << "going to leave at " << getPauseEndTime() << std::endl;
			return;
		}*/
	}
	
	//Otherwise we wake up
	else {
		didSleep = true;
		setPauseEndTime(7);
		OSSObjectLog << "waking up..." << std::endl;
		setAsleep(false);
		return;
	}
	
	//Leave
	OSSObjectLog << "leaving" << std::endl;
	setNextDestination(randd(tower->time + 0.25, 10), NULL);
}

bool HotelGuest::isLeaving()
{
	return didSleep;
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
