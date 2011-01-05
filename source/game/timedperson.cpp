#include "timedperson.h"
#include "tower.h"

using namespace OSS;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

TimedPerson::TimedPerson(Tower * tower) : Person(tower) 
{
	nextDestinationTime = 0;
	nextDestinationValid = false;
	nextDestinationPauseDuration = 0;
	
	pauseDurationAtDestination = 0;
	pauseDuration = 0;
	pauseEndTime = 0;
}

void TimedPerson::reset()
{
	Person::reset();
	
	clearNextDestination();
	setPauseDurationAtDestination(0);
	setPauseDuration(0);
	setPauseEndTime(0);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Next Destination
//----------------------------------------------------------------------------------------------------

Item * TimedPerson::getNextDestination()
{
	return nextDestination;
}

double TimedPerson::getNextDestinationTime()
{
	return nextDestinationTime;
}

double TimedPerson::getNextDestinationPauseDuration()
{
	return nextDestinationPauseDuration;
}

bool TimedPerson::isNextDestinationValid()
{
	return nextDestinationValid;
}



void TimedPerson::setNextDestination(Item * destination)
{
	setNextDestination(0, destination, 0);
}

void TimedPerson::setNextDestination(Item * destination, double duration)
{
	setNextDestination(0, destination, duration);
}

void TimedPerson::setNextDestination(double time, Item * destination)
{
	setNextDestination(time, destination, 0);
}

void TimedPerson::setNextDestination(double time, Item * destination, double duration)
{
	if (nextDestinationTime != time ||
		nextDestination != destination ||
		nextDestinationPauseDuration != duration) {
		
		//Store the values
		nextDestinationTime = time;
		nextDestination = destination;
		nextDestinationPauseDuration = duration;
		
		//Mark the destination as valid
		nextDestinationValid = true;
		
		//Update the destination itself
		updateTimedDestination();
	}
}

void TimedPerson::clearNextDestination()
{
	nextDestinationValid = false;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Route
//----------------------------------------------------------------------------------------------------

void TimedPerson::updateTimedDestination()
{
	if (isNextDestinationValid() && getNextDestinationTime() <= tower->time) {
		setDestination(getNextDestination());
		setPauseDurationAtDestination(getNextDestinationPauseDuration());
		clearNextDestination();
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Location
//----------------------------------------------------------------------------------------------------

double TimedPerson::getPauseDurationAtDestination()
{
	return pauseDurationAtDestination;
}

void TimedPerson::setPauseDurationAtDestination(double duration)
{
	pauseDurationAtDestination = duration;
}

double TimedPerson::getPauseDuration()
{
	return pauseDuration;
}

void TimedPerson::setPauseDuration(double duration)
{
	//if (pauseDuration != duration) {
	pauseDuration = duration;
	setPauseEndTime(tower->time + duration);
	//}
}

double TimedPerson::getPauseEndTime()
{
	return pauseEndTime;
}

void TimedPerson::setPauseEndTime(double time)
{
	pauseEndTime = time;
}

bool TimedPerson::isPausing()
{
	return (tower->time <= getPauseEndTime());
}

bool TimedPerson::hasNoPlans()
{
	return (!isPausing() && !isNextDestinationValid() && !hasRoute());
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Notifications
//----------------------------------------------------------------------------------------------------

void TimedPerson::onArrivedAtDestination()
{
	//Calculate the time until the person will stay here
	setPauseDuration(getPauseDurationAtDestination());
	
	//Update the rest
	Person::onArrivedAtDestination();
}
