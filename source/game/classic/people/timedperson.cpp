#include "timedperson.h"

using namespace OSS;
using namespace Classic;





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
	}
}

void TimedPerson::clearNextDestination()
{
	nextDestinationValid = false;
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
	setPauseEndTime(tower->time->getTime() + duration);
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

void TimedPerson::setPauseEndTimeDaily(double time)
{
	unsigned int baseTime = tower->time->getTime() / 24;
	baseTime *= 24;
	setPauseEndTime(baseTime + time);
}

void TimedPerson::setPauseEndTimeDailyFuture(double time)
{
	if (tower->time->getTimeOfDay() >= time)
		time += 24;
	setPauseEndTimeDaily(time);
}

bool TimedPerson::isPausing()
{
	return (tower->time->getTime() <= getPauseEndTime());
}

bool TimedPerson::hasNoPlans()
{
	return (!isPausing() && !isNextDestinationValid() && !hasRoute());
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Location
//----------------------------------------------------------------------------------------------------

void TimedPerson::didMoveToItem()
{
	//Check if the we arrived at our destination.
	if (isAtDestination()) {
		//Calculate the time until which the person will stay here
		setPauseDuration(getPauseDurationAtDestination());
	}

	Person::didMoveToItem();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

void TimedPerson::advance(double dt)
{
	Person::advance(dt);
	
	//If there's a next destination and the time is right, advance
	if (isNextDestinationValid() && getNextDestinationTime() <= tower->time->getTime()) {
		setDestination(getNextDestination());
		setPauseDurationAtDestination(getNextDestinationPauseDuration());
		clearNextDestination();
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark State
//----------------------------------------------------------------------------------------------------

void TimedPerson::update()
{
	Person::update();
	
	//If the person has no more plans, think!
	if (hasNoPlans())
		updateNextDestination();
}

void TimedPerson::updateNextDestination()
{
}
