#include "person.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

Person::Person(Tower * tower) : tower(tower),
updateRouteIfNeeded(this, &Person::updateRoute, &updateIfNeeded),
updateAnimationIfNeeded(this, &Person::updateAnimation, &updateIfNeeded)
{
	assert(tower);
	
	//Add us to the tower so we get simulated and updated appropriately.
	tower->environment->addPerson(this);
	
	//Reset all member variables
	floor = 0;
	arrivalTime = 0;
	type = kManType;
	stress = 0;
	pauseEndTime = 0;
}

string Person::className() const
{
	string type = getTypeName();
	if (type.length() > 0)
		return type;
	return GameObject::className();
}

Person::~Person()
{
	//Remove us from the tower.
	tower->environment->removePerson(this);
	
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
		//Decide whether we're moving into the tower or out of the tower
		bool movingIn = (!item && i);
		bool movingOut = (item && !i);
		
		//Remove the person from the current item
		if (item) item->removePerson(this);
		
		//Switch to the new item
		item = i;
		
		//Update the arrival time
		setArrivalTime(tower->time->getTime());
		
		//Add person to the new item
		if (item) item->addPerson(this);
		
		//If we're moving into the tower, add us to the appropriate list. Vice versa otherwise.
		if (movingIn) tower->environment->addPersonToTower(this);
		if (movingOut) tower->environment->removePersonFromTower(this);
		
		//Update the route
		updateRouteIfNeeded.setNeeded();
		
		//Also update the animation
		updateAnimationIfNeeded.setNeeded();
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
		setPauseEndTime(0);
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
	Route::Node * node = route->nextNode();;
	return node;
}

int Person::getStartFloor()
{
	if (!getRouteNode())
		return 0;
	return getRouteNode()->start.origin.y;
}

int Person::getEndFloor()
{
	if (!getRouteNode())
		return 0;
	recti end = getRouteNode()->end;
	return end.origin.y;
}

bool Person::isAtRouteNodeTransport()
{
	if (!getRouteNode())
		return false;
	return isAt(getRouteNode()->transport);
}

bool Person::isOnStartFloor()
{
	Route::Node * node = getRouteNode();
	if (!node)
		return false;
	return (getFloor() == node->start.origin.y);
}

bool Person::isOnEndFloor()
{
	Route::Node * node = getRouteNode();
	if (!node)
		return false;
	return (getFloor() == node->end.origin.y);
}



double Person::getPauseEndTime()
{
	return pauseEndTime;
}

double Person::getPauseDuration()
{
	return (getPauseEndTime() - getArrivalTime());
}

void Person::setPauseEndTime(double t)
{
	if (pauseEndTime != t) {
		pauseEndTime = t;
	}
}

void Person::setPauseEndTimeToday(double t)
{
	setPauseEndTime(tower->time->getLogicalStartOfDay() + t);
}

void Person::setPauseEndTimeTomorrow(double t)
{
	setPauseEndTimeToday(t + 24);
}

void Person::setPauseDuration(double d)
{
	setPauseEndTime(/*getArrivalTime()*/tower->time->getTime() + d);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

void Person::advance(double dt)
{
	GameObject::advance(dt);
	
	//Advance the route
	advanceRoute(dt);
	
	//Advance the animation if we're supposed to
	if (shouldAnimate())
		advanceAnimation(dt);
}

void Person::advanceRoute(double dt)
{
	//If we are at our destination, we need some new plans.
	if (isAtDestination()) {
		
		//Check if the pause has passed
		if (tower->time->getTime() >= getPauseEndTime()) {
			
			//Make the person think where to go next.
			think();
		}
	}
}

void Person::advanceAnimation(double dt)
{
	//Update the animation, which means that the animation index and location get changed.
	if (shouldAdvance("animation", getAnimationPeriod()))
		updateAnimationIfNeeded.setNeeded();
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
	
	//Update the animation if required
	updateAnimationIfNeeded();
}

void Person::updateRoute()
{
	//Check whether we are not at our destination which means we have to travel around.
	if (!isAtDestination()) {
		
		//Check whether the route is valid for our current destination. We do this by checking
		//whether the route's destination equals our destination's rect, and that we are either on
		//the current route node's start or end floor.
		bool destinationRectChanged = (route && route->destination != getDestinationRect());
		bool neitherOnStartNorEndFloor = (!isOnStartFloor() && !isOnEndFloor());
		if (!route || destinationRectChanged || neitherOnStartNorEndFloor) {
			
			//Calculate a new route to the destination.
			route = Route::findRoute(tower, getItemRect(), getDestinationRect(),
									 routeFindingOptions());
			
			//If we were not able to find a round, post an error message and reset the person to the
			//lobby.
			if (!route) {
				OSSObjectError << "can't find route from " << getItemRect().description()
				<< " to " << getDestinationRect().description() << "!" << std::endl;
				setItem(NULL);
				setDestination(NULL);
				setFloor(0);
			}
			
			//Otherwise do the first step of the route to get us going.
			else {
				if (getRouteNode())
					setItem(getRouteNode()->transport);
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
		}
		
		//Check whether we have arrived on the route's end floor. If we did, we may move on to the
		//destination item.
		if (route && getFloor() == route->destination.minY())
			setItem(getDestination());
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

void Person::drawAnimation(rectd dirtyRect)
{
	if (animationSprite && shouldAnimate())
		animationSprite->draw();
}

Texture * Person::getQueueingTexture()
{
	return Texture::named("simtower/people/queueing");
}

rectd Person::getQueueingTextureRect()
{
	//Since the queueing texture is organized into slice of the same width, we may just calculate
	//the slice index for a given person type and calculate the texture rect from that.
	unsigned int slice = 0;
	switch (getType()) {
		case kManType:				slice = 0; break;
		case kSalesmanType:			slice = 1; break;
		case kWomanAType:			slice = 2; break;
		case kChildType:			slice = 3; break;
		case kWomanBType:			slice = 4; break;
		case kJanitorType:			slice = 5; break;
		case kMotherWithChildType:	slice = 6; break;
		case kMotherType:			slice = 7; break;
	}
	
	//Calculate the texture rect
	return rectd(slice / 8.0, 0, 0.125, 1);
}

double Person::getQueueingWidth()
{
	switch (getType()) {
		case kJanitorType:
		case kMotherWithChildType:
		case kMotherType:
			return 16;
			break;
			
		default:
			return 8;
			break;
	}
}
