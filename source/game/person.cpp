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
	floor = 0;
	nextFloor = 0;
	nodeIndex = 0;
	arrivalTime = 0;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Location
//----------------------------------------------------------------------------------------------------

int Person::getFloor()
{
	return floor;
}

void Person::setFloor(int floor)
{
	this->floor = floor;
}

Item * Person::getItem() const
{
	return item;
}

void Person::setItem(Item * item)
{
	if (this->item != item) {
		//Remove the person from the current item
		if (this->item)
			this->item->removePerson(this);
		
		//Switch to the new item
		this->item = item;
		
		//Update the arrival time
		setArrivalTime(tower->time);
		
		//Add person to the new item
		if (this->item)
			this->item->addPerson(this);
	}
}

double Person::getArrivalTime()
{
	//Wrap around the arrival time if required. If the person arrived yesterday at 17:00 and it is
	//10:00 today, the time since arrival would be -7h, which makes no sense. Instead it should
	//read as 17h.
	if (arrivalTime < tower->time)
		arrivalTime -= 24;
	return arrivalTime;
}

void Person::setArrivalTime(double time)
{
	arrivalTime = time;
}

double Person::getTimeSinceArrival()
{
	return (tower->time - getArrivalTime());
}

bool Person::isAt(Item * item)
{
	return (getItem() == item);
}

bool Person::hasBeenAtFor(Item * item, double duration)
{
	return (getItem() == item && getTimeSinceArrival() >= duration);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Journey
//----------------------------------------------------------------------------------------------------

int Person::getNextFloor()
{
	return nextFloor;
}

void Person::setNextFloor(int nextFloor)
{
	this->nextFloor = nextFloor;
}

void Person::initJourney()
{
	nextFloor = getFloor();
	nodeIndex = 0;
	advanceJourney();
}

void Person::advanceJourney()
{
	assert(getRoute());
	
	//Check whether there are nodes left in the route
	const Route::Nodes * nodes = &getRoute()->getNodes();
	if (nodes->size() > nodeIndex) {
		
		//Fetch the next node
		const Route::Node * node = &nodes->at(nodeIndex);
		
		//Extract the floor information
		setFloor(node->start.minY());
		setNextFloor(node->end.minY());
		
		//Attach to the transport
		assert(node->transport);
		setItem(node->transport);
		
		//Increase the node index
		nodeIndex++;
	}
	
	//No more route nodes
	else {
		onArrivedAtDestination();
	}
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
		if (route)
			initJourney();
	}
}

void Person::updateRoute()
{
	//Calculate the starting rect of the root. It's either the current item's rect or the ground
	//floor.
	recti start = (getItem() ? getItem()->getRect() : tower->getGroundFloorRect());
	
	//The destination rect likewise
	recti destination = (getDestination() ? getDestination()->getRect() : tower->getGroundFloorRect());
	
	//If there's no current item, reset the current floor to the ground level.
	if (!getItem())
		setFloor(0);
	
	//Find a route to the destination
	Route * route = tower->findRoute(start, destination);
	setRoute(route);
	if (!route)
		OSSObjectError << "cannot find route from " << start.description()
		<< " to " << destination.description() << std::endl;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Notifications
//----------------------------------------------------------------------------------------------------

void Person::onArrivedAtDestination()
{
	//Move to the destination item
	setItem(getDestination());
	
	//Reset the route
	setRoute(NULL);
}
