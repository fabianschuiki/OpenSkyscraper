#include "officeworker.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

OfficeWorker::OfficeWorker(Tower * tower, OfficeItem * office) : Person(tower), office(office)
{
	reset();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Animation
//----------------------------------------------------------------------------------------------------

void OfficeWorker::updateAnimation()
{
	Person::updateAnimation();
	if (!shouldAnimate())
		return;
	
	//Inititalize the animation sprite if not done so yet.
	if (!animationSprite) {
		animationSprite = new Sprite;
		
		//Load the texture and set the slice size.
		animationSprite->texture = Texture::named("simtower/facilities/office/workers");
		animationSprite->rect = rectd(0, 0, 16, 24);
	}
}

bool OfficeWorker::shouldAnimate()
{
	return isAt(office);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

void OfficeWorker::reset()
{
	arrivalTime = 0;
	didChooseArrivalTime = false;
	hasArrived = false;
	
	departureTime = 0;
	didChooseDepartureTime = false;
	hasDeparted = false;
}

void OfficeWorker::think()
{
	Person::think();
	
	//Skip weekends.
	if (tower->time->isWeekend())
		setPauseEndTimeToday(24 + 5);
	
	//Choose the arrival time in the morning.
	if (!didChooseArrivalTime) {
		arrivalTime = tower->time->getLogicalTodayRandom(7, 10);
		didChooseArrivalTime = true;
		setPauseEndTime(arrivalTime);
		return;
	}
	
	//Arrive in the morning.
	if (!hasArrived) {
		if (!isAt(office)) {
			setDestination(office);
			return;
		} else {
			hasArrived = true;
			setPauseDuration(0.5);
			return;
		}
	}
	
	//Decide when to go home.
	if (!didChooseDepartureTime) {
		departureTime = tower->time->getLogicalTodayRandom(17, 20);
		didChooseDepartureTime = true;
		setPauseEndTime(departureTime);
		return;
	}
	
	//Go home.
	if (!hasDeparted) {
		if (!isAt(NULL)) {
			setDestination(NULL);
			return;
		} else {
			hasDeparted = true;
			setPauseEndTimeToday(24 + 5);
			return;
		}
	}
	
	//Reset the state machine.
	reset();
}

