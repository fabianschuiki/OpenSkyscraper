#include "person.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

Person::Person(Tower * tower) : tower(tower),
updateRouteIfNeeded(this, &Person::updateRoute, &updateIfNeeded)
{
	assert(tower);
	
	//Reset all member variables
	floor = 0;
	arrivalTime = 0;
	type = kManType;
	stress = 0;
}

Person::~Person()
{
	//Make sure we're removed from the current item's people list.
	setItem(NULL);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Location
//----------------------------------------------------------------------------------------------------

int Person::getFloor()
{
	return floor;
}

void Person::setFloor(int f)
{
	if (floor != f) {
		floor = f;
		updateRouteIfNeeded.setNeeded();
	}
}



Item * Person::getItem() const
{
	return item;
}

void Person::setItem(Item * i)
{
	if (item != i) {		
		//Remove the person from the current item
		if (item) item->removePerson(this);
		
		//Switch to the new item
		item = i;
		
		//Update the arrival time
		setArrivalTime(tower->time->getTime());
		
		//Add person to the new item
		if (item) item->addPerson(this);
		
		//Update the route
		updateRouteIfNeeded.setNeeded();
	}
}

recti Person::getItemRect()
{
	if (getItem())
		return getItem()->getRect();
	return tower->getGroundFloorRect();
}

bool Person::isAt(Item * i)
{
	return (getItem() == i);
}



double Person::getArrivalTime()
{
	return arrivalTime;
}

void Person::setArrivalTime(double time)
{
	arrivalTime = time;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Type
//----------------------------------------------------------------------------------------------------

Person::Type Person::getType()
{
	return type;
}

void Person::setType(Type t)
{
	if (type != t) {
		willChangeType(t);
		type = t;
		didChangeType();
	}
}



Person::Gender Person::getGender()
{
	if (getType() >= kWomanAType)
		return kFemale;
	return kMale;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Stress
//----------------------------------------------------------------------------------------------------

double Person::getStress()
{
	return stress;
}

void Person::setStress(double s)
{
	if (stress != s) {
		willChangeStress(s);
		stress = s;
		didChangeStress();
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Journey
//----------------------------------------------------------------------------------------------------

Item * Person::getDestination()
{
	return destination;
}

void Person::setDestination(Item * d)
{
	if (destination != d) {
		willChangeDestination(d);
		destination = d;
		didChangeDestination();
		updateRouteIfNeeded.setNeeded();
	}
}

recti Person::getDestinationRect()
{
	if (getDestination())
		return getDestination()->getRect();
	return tower->getGroundFloorRect();
}

bool Person::isAtDestination()
{
	return isAt(getDestination());
}



bool Person::hasRoute()
{
	return (route != NULL);
}

Route::Node * Person::getRouteNode()
{
	if (!route)
		return NULL;
	return route->nextNode();
}

bool Person::isAtRouteNodeTransport()
{
	if (!getRouteNode())
		return false;
	return isAt(getRouteNode()->transport);
}

bool Person::isOnStartFloor()
{
	if (!getRouteNode())
		return false;
	return (getFloor() == getRouteNode()->start.origin.y);
}

bool Person::isOnEndFloor()
{
	if (!getRouteNode())
		return false;
	return (getFloor() == getRouteNode()->end.origin.y);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

void Person::advance(double dt)
{
	GameObject::advance(dt);
}

bool Person::shouldBeAnimated()
{
	return false;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark State
//----------------------------------------------------------------------------------------------------

void Person::update()
{
	GameObject::update();
	
	//Update the route if required
	updateRouteIfNeeded();
}

void Person::updateRoute()
{
	//Check whether we are not at our destination which means we have to travel around.
	if (!isAtDestination()) {
		
		//Check whether the route is valid for our current destination. We do this by checking
		//whether the route's destination equals our destination's rect, and that we are either on
		//the current route node's start or end floor.
		if (!route || route->destination != getDestinationRect() ||
			(!isOnStartFloor() && isOnEndFloor())) {
			
			//Calculate a new route to the destination.
			route = Route::findRoute(tower, getItemRect(), getDestinationRect());
			
			//If we were not able to find a round, post an error message and reset the person to the
			//lobby.
			if (!route) {
				OSSObjectError << "can't find route from " << getItemRect().description()
				<< " to " << getDestinationRect().description() << "!" << std::endl;
				setItem(NULL);
				setDestination(NULL);
				setFloor(0);
			}
		}
		
		//Check whether it is time to advance to the next route node. This is accomplished by
		//checking whether the route is set and we are on the route node's end floor.
		if (route && isOnEndFloor()) {
			
			//Get rid of the current route node.
			route->popNode();
			
			//If there is a next node, move to its transport.
			if (getRouteNode())
				setItem(getRouteNode()->transport);
			
			//Otherwise add us to the destination since we obviously arrived there.
			else
				setItem(getDestination());
		}
	}
	
	//Otherwise we may simply get rid of the route for good.
	else {
		route = NULL;
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

void Person::draw(rectd dirtyRect)
{
	GameObject::draw(dirtyRect);
}

bool Person::shouldBeDrawn()
{
	return false;
}
