#include "person.h"
#include "item.h"
#include "tower.h"

using namespace OSS;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

Person::Person(Tower * tower) : tower(tower), CoreObject()
{
	currentFloor = 0;
	nextFloor = 0;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Journey
//----------------------------------------------------------------------------------------------------

void Person::initJourney()
{
}

void Person::advanceJourney()
{
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Route
//----------------------------------------------------------------------------------------------------

Item * Person::getDestination()
{
	return destination;
}

void Person::setDestination(Item * destination)
{
	if (this->destination != destination) {
		this->destination = destination;
		if (destination)
			OSSObjectLog << "changed destination to " << destination->description() << std::endl;
		else
			OSSObjectLog << "changed destination to [out]" << std::endl;

		updateRoute();
	}
}

Route * Person::getRoute()
{
	return route;
}

void Person::setRoute(Route * route)
{
	if (this->route != route) {
		this->route = route;
		initJourney();
	}
}

void Person::updateRoute()
{
	//Calculate the starting rect of the root. It's either the current item's rect or the ground
	//floor.
	recti start = (currentItem ? currentItem->getRect() : tower->getGroundFloorRect());
	
	//The destination rect likewise
	recti destination = (getDestination() ? getDestination()->getRect() : tower->getGroundFloorRect());
	
	//If there's no current item, reset the current floor to the ground level.
	if (!currentItem)
		currentFloor = 0;
	
	//Find a route to the destination
	Route * route = tower->findRoute(start, destination);
	setRoute(route);
	if (!route)
		OSSObjectError << "cannot find route from " << start.description()
		<< " to " << destination.description() << std::endl;
}
