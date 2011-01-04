#include "timedperson.h"
#include "tower.h"

using namespace OSS;


Item * TimedPerson::getNextDestination()
{
	return nextDestination;
}

double TimedPerson::getNextDestinationTime()
{
	return nextDestinationTime;
}

void TimedPerson::setNextDestination(double time, Item * destination)
{
	if (nextDestinationTime != time || nextDestination != destination) {
		nextDestinationTime = time;
		nextDestination = destination;
		nextDestinationValid = true;
		updateTimedJourney();
	}
}

void TimedPerson::clearNextDestination()
{
	nextDestinationValid = false;
}

void TimedPerson::updateTimedJourney()
{
	if (hasNextDestination() && getNextDestinationTime() <= tower->time) {
		setDestination(getNextDestination());
		clearNextDestination();
	}
}

bool TimedPerson::hasNextDestination()
{
	return nextDestinationValid;
}
