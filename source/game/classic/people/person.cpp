#include "person.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

Person::Person(Tower * tower) : Core::Object(), tower(tower)
{
	assert(tower);
	
	//Attributes
	type = kManType;
	stress = 0;
	
	//Animation Sprite
	animationTime = 0;
	animationIndex = 0;
	
	//Location
	floor = 0;
	
	//Journey
	nextFloor = 0;
	nodeIndex = 0;
	
	//Route
	arrivalTime = 0;
}

void Person::reset()
{
	setItem(NULL);
	setDestination(NULL);
	
	//DEBUG: Reset the properties
	boolProps.clear();
	intProps.clear();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Attributes
//----------------------------------------------------------------------------------------------------

Person::Type Person::getType()
{
	return type;
}

void Person::setType(Type type)
{
	if (this->type != type) {
		this->type = type;
		onChangeType();
	}
}

void Person::onChangeType()
{
	updateManagedSprites();
}

Person::Gender Person::getGender()
{
	if (getType() >= kWomanAType)
		return kFemale;
	return kMale;
}



double Person::getStress()
{
	return stress;
}

void Person::setStress(double stress)
{
	if (this->stress != stress) {
		this->stress = stress;
		onChangeStress();
	}
}

void Person::onChangeStress()
{
	updateManagedSprites();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Managed Sprites
//----------------------------------------------------------------------------------------------------

void Person::addManagedSprite(Sprite * sprite, SpriteType type, SpriteHeading heading)
{
	managedSprites.insert(sprite);
	managedSpriteTypes[sprite] = type;
	managedSpriteHeadings[sprite] = heading;
	initManagedSprite(sprite);
}

void Person::removeManagedSprite(Sprite * sprite)
{
	managedSprites.erase(sprite);
	managedSpriteTypes.erase(sprite);
	managedSpriteHeadings.erase(sprite);
}



void Person::setManagedSpriteType(Sprite * sprite, SpriteType type)
{
	if (managedSpriteTypes[sprite] != type) {
		managedSpriteTypes[sprite] = type;
		updateManagedSprite(sprite);
	}
}

void Person::setManagedSpriteHeading(Sprite * sprite, SpriteHeading heading)
{
	if (managedSpriteHeadings[sprite] != heading) {
		managedSpriteHeadings[sprite] = heading;
		updateManagedSprite(sprite);
	}
}



void Person::initManagedSprite(Sprite * sprite)
{
}

void Person::updateManagedSprite(Sprite * sprite)
{
}

void Person::updateManagedSprites()
{
	for (SpriteSet::iterator i = managedSprites.begin(); i != managedSprites.end(); i++)
		updateManagedSprite(*i);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Animation Sprite
//----------------------------------------------------------------------------------------------------

Sprite & Person::getAnimationSprite()
{
	return animationSprite;
}



unsigned int Person::getAnimationIndex()
{
	return animationIndex;
}

void Person::setAnimationIndex(unsigned int animationIndex)
{
	if (this->animationIndex != animationIndex) {
		this->animationIndex = animationIndex;
		onChangeAnimationIndex();
	}
}

void Person::onChangeAnimationIndex()
{
	updateAnimationSprite();
}



const int2 & Person::getAnimationLocation()
{
	return animationLocation;
}

void Person::setAnimationLocation(int2 animationLocation)
{
	if (this->animationLocation != animationLocation) {
		this->animationLocation = animationLocation;
		onChangeAnimationLocation();
	}
}

void Person::onChangeAnimationLocation()
{
	updateAnimationSprite();
}



void Person::initAnimationSprite()
{
	updateAnimationSprite();
}

void Person::updateAnimationSprite()
{
	if (!getItem())
		return;
	
	//Position the animation sprite
	rectd worldRect = animationSprite.getRect();
	worldRect.origin = getItem()->getWorldRect().origin;
	worldRect.origin += tower->convertCellToWorldCoordinates(getAnimationLocation());
	animationSprite.setRect(worldRect);
}



double Person::getAnimationPeriod()
{
	return 0.75;
}

bool Person::shouldAnimate()
{
	return false;
}

void Person::resetAnimation()
{
	animationTime = 0;
	if (shouldAnimate())
		shuffleAnimation();
}

void Person::advanceAnimation(double dt)
{
	if (!shouldAnimate())
		return;
	
	//Increase the animation time
	animationTime += dt;
	
	//If we passed the animation period, shuffle the animation
	double period = getAnimationPeriod();
	if (animationTime >= period) {
		animationTime -= period;
		shuffleAnimation();
	}
}

void Person::shuffleAnimation()
{
}

void Person::drawAnimation(rectd visibleRect)
{
	if (shouldAnimate())
		animationSprite.draw(visibleRect);
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
		//Retain us in case the retain count drops to 0 during the switch
		retain();
		
		//Remove the person from the current item
		if (this->item)
			this->item->removePerson(this);
		
		//Switch to the new item
		this->item = item;
		
		//Update the arrival time
		setArrivalTime(tower->time->getTime());
		
		//Add person to the new item
		if (this->item)
			this->item->addPerson(this);
		
		//Reset the animation
		resetAnimation();
		
		//Compensate the retain
		release();
	}
}

double Person::getArrivalTime()
{
	//Wrap around the arrival time if required. If the person arrived yesterday at 17:00 and it is
	//10:00 today, the time since arrival would be -7h, which makes no sense. Instead it should
	//read as 17h.
	if (arrivalTime < tower->time->getTime())
		arrivalTime -= 24;
	return arrivalTime;
}

void Person::setArrivalTime(double time)
{
	arrivalTime = time;
}

double Person::getTimeSinceArrival()
{
	return (tower->time->getTime() - getArrivalTime());
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
		setItem((Item *)((TransportItem *)node->transport));
		
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
	/*if (!getItem())
		setFloor(0);*/
	
	//If the item and destination are equal, set the route to null
	if (getItem() == getDestination())
		setRoute(NULL);
	
	//Otherwise find a route to the destination
	else {
		Route * route = Route::findRoute(tower, start, destination);
		setRoute(route);
		if (!route)
			OSSObjectError << "cannot find route from " << start.description()
			<< " to " << destination.description() << std::endl;
	}
}

bool Person::isAtDestination()
{
	return isAt(getDestination());
}

bool Person::hasRoute()
{
	return (getRoute() != NULL);
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





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Intelligence
//----------------------------------------------------------------------------------------------------

void Person::update()
{
}
