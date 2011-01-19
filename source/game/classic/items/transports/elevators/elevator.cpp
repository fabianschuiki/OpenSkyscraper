#include "elevator.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

ElevatorItem::ElevatorItem(Tower * tower, ItemDescriptor * descriptor)
: TransportItem(tower, descriptor),
respondToCallsIfNeeded(this, &ElevatorItem::respondToCalls, &updateItemIfNeeded),
updateQueuesIfNeeded(this, &ElevatorItem::updateQueues, &updateItemIfNeeded),
updateQueueLocationsIfNeeded(this, &ElevatorItem::updateQueueLocations, &updateQueuesIfNeeded)
{
	animationFrame = 0;
	
	//DEBUG: reset the debugfloor
	debugFloor = 0;
	
	//Load the required textures
	backgroundNormal		= Texture::named("simtower/transport/floordigits/one/normal");
	backgroundHighlighted	= Texture::named("simtower/transport/floordigits/one/highlighted");
	lsNormal				= Texture::named("simtower/transport/floordigits/two/ls/normal");
	lsHighlighted			= Texture::named("simtower/transport/floordigits/two/ls/highlighted");
	msNormal				= Texture::named("simtower/transport/floordigits/two/ms/normal");
	msHighlighted			= Texture::named("simtower/transport/floordigits/two/ms/highlighted");
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Queues
//----------------------------------------------------------------------------------------------------

ElevatorQueue * ElevatorItem::getQueue(int floor, Direction dir)
{
	//Initialize the queue if it doesn't exist yet
	if (!queues[floor][dir]) {
		
		//Create the queue and set the appropriate direction.
		ElevatorQueue * q = new ElevatorQueue(this);
		q->setDirection(dir);
		
		//Make sure update requirements of the queue are being propagated to us.
		q->updateIfNeeded.parent = &updateQueuesIfNeeded;
		
		//Store the queue and mark the queue locations as to be updated.
		queues[floor][dir] = q;
		updateQueueLocationsIfNeeded.setNeeded();
	}
	return queues[floor][dir];
}

ElevatorQueue * ElevatorItem::getMostUrgentQueue()
{
	//Iterate through the queues and find the one with the highest wait duration.
	ElevatorQueue * most = NULL;
	for (QueueMap::iterator qm = queues.begin(); qm != queues.end(); qm++)
		for (QueuePair::iterator qp = qm->second.begin(); qp != qm->second.end(); qp++)
			if (qp->second->isCalled())
				if (!most || most->getWaitDuration() < qp->second->getWaitDuration())
					most = qp->second;
	
	//Return whatever we have or haven't found.
	return most;
}

ElevatorQueue * ElevatorItem::getNextQueue(ElevatorCar * car)
{
	if (!car) return NULL;
	
	//Iterage through all the queues with the same direction as the car and return the one closest
	//to the car which is in the cars direction.
	ElevatorQueue * closest = NULL;
	double closestDistance;
	for (QueueMap::iterator qm = queues.begin(); qm != queues.end(); qm++) {
		
		//If there's no queue for the car's direction, skip ahead.
		if (!qm->second.count(car->getDirection()))
			continue;
		
		//Get the queue for the car's direction
		ElevatorQueue * q = qm->second[car->getDirection()];
		assert(q); //q must exist now due to the prechecking of the map the line above.
		
		//Calculate the distance (signed) between the car and the queue.
		double distance = qm->first - car->getFloor();
		
		//If the distance multiplied by the car's direction (1 for up, -1 for down) is negative,
		//the queue does not lie in the car's path, so we have to skip it. Addendum: It is actually
		//better to check if the distance is lower than 1. This catches all calls behind the car
		//but also calls on the current floor which the car already had good reasons not to answer.
		if (distance * car->getDirection() < 1)
			continue;
		
		//Compare the queue to the one we already found
		if (!closest || closestDistance > distance) {
			closestDistance = distance;
			closest = q;
		}
	}
	
	//Return whatever we've found
	return closest;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Cars
//----------------------------------------------------------------------------------------------------

void ElevatorItem::addCar(ElevatorCar * car)
{
	//Insert the car
	cars.insert(car);
	
	//Make sure the udpate need propagation works
	car->updateIfNeeded.parent = &updateItemIfNeeded;
}

void ElevatorItem::addCar(int floor)
{
	//Create the new car
	ElevatorCar * car = new ElevatorCar(this);
	
	//Transfer the funds
	tower->funds->transfer(-getCarPrice());
	
	//Add the car
	addCar(car);
}

void ElevatorItem::removeCar(ElevatorCar * car)
{
	//Disable the update propagation
	car->updateIfNeeded.parent = NULL;
	
	//Remove the car
	cars.erase(car);
}

void ElevatorItem::respondToCalls()
{
	//Iterate as long as there are queues to be responded to.
	ElevatorQueue * q;
	while ((q = getMostUrgentQueue())) {
		
		//Find the car closest to the queue's location.
		ElevatorCar * car = getIdleCar(q->getRect().minY());
		
		//If we weren't able to find a car, we break out of the loop since obviously all cars are
		//busy.
		if (!car) break;
		
		//Otherwise set the car's direction and send it off to the queue.
		car->answerCall(q);
	}
}

ElevatorCar * ElevatorItem::getIdleCar(int floor)
{
	//Iterate through all the cars and find the one closest to the given floor.
	ElevatorCar * car = NULL;
	for (CarSet::iterator it = cars.begin(); it != cars.end(); it++)
		if ((*it)->isIdle())
			if (!car || fabs(car->getFloor() - floor) > fabs((*it)->getFloor() - floor))
				car = *it;
	
	//Return whatever car we've found
	return car;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Layout
//----------------------------------------------------------------------------------------------------

bool ElevatorItem::isFloorActive(int floor)
{
	return true;
}

bool ElevatorItem::isFloorHighlighted(int floor)
{
	return (floor == debugFloor);
}

recti ElevatorItem::getMotorRect()
{
	recti r = getRect();
	r.origin.y = r.maxY();
	r.size.y = 1;
	return r;
}

recti ElevatorItem::getBufferRect()
{
	recti r = getRect();
	r.origin.y = r.minY() - 1;
	r.size.y = 1;
	return r;
}

recti ElevatorItem::getCarsRect()
{
	return getRect();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------


unsigned int ElevatorItem::getAnimationFrame()
{
	return animationFrame;
}

void ElevatorItem::setAnimationFrame(unsigned int af)
{
	if (animationFrame != af) {
		animationFrame = af;
		updateBackgroundIfNeeded.setNeeded();
	}
}

void ElevatorItem::advanceItem(double dt)
{
	TransportItem::advanceItem(dt);
	
	//Advance the animation
	if (shouldAnimate() && shouldAdvance("animation", 1.0 / 3))
		setAnimationFrame((getAnimationFrame() + 1) % 3);
	
	//Advance the cars
	for (CarSet::iterator it = cars.begin(); it != cars.end(); it++)
		(*it)->advance(dt);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark State
//----------------------------------------------------------------------------------------------------

void ElevatorItem::updateItem()
{
	TransportItem::updateItem();
	
	//Update the cars
	for (CarSet::iterator it = cars.begin(); it != cars.end(); it++)
		(*it)->updateIfNeeded();
	
	//Update the queues
	updateQueuesIfNeeded();
	
	//Respond to calls
	respondToCallsIfNeeded();
}

void ElevatorItem::updateBackground()
{
	//Note that we don't call the superclass's updateBackground since that would initialize the
	//background sprites which we don't want for the elevator since we're drawing the complicated
	//background ourselves without sprites.
	
	//Initialize the motor and buffer sprites if required
	if (!motorSprite)
		motorSprite = new Sprite;
	if (!bufferSprite)
		bufferSprite = new Sprite;
	
	//The motor is the left, the buffer the right half of the texture
	motorSprite->textureRect = rectd(0, 0, 0.5, 1);
	bufferSprite->textureRect = rectd(0.5, 0, 0.5, 1);
	
	//Position the motor and buffer
	motorSprite->rect = (tower->structure->cellToWorld(getMotorRect()));
	bufferSprite->rect = (tower->structure->cellToWorld(getBufferRect()));
	
	//Load the appropriate textures
	stringstream textureName;
	textureName << "simtower/transport/elevator/";
	textureName << getMotorBufferTypeName();
	textureName << "/motorbuffer/";
	textureName << getAnimationFrame();
	motorSprite->texture = Texture::named(textureName.str());
	bufferSprite->texture = motorSprite->texture;
}

void ElevatorItem::updateQueues()
{
	//Update the queue locations if needed
	updateQueueLocationsIfNeeded();
	
	//Update the queues themselves
	for (QueueMap::iterator qm = queues.begin(); qm != queues.end(); qm++)
		for (QueuePair::iterator qp = qm->second.begin(); qp != qm->second.end(); qp++)
			qp->second->updateIfNeeded();
}

void ElevatorItem::updateQueueLocations()
{
	//Iterate through the queues and calculate each queue's rect
	for (QueueMap::iterator qm = queues.begin(); qm != queues.end(); qm++) {
		for (QueuePair::iterator qp = qm->second.begin(); qp != qm->second.end(); qp++) {
			
			//Calculate the rect for this queue
			//TODO: do this the right way by finding other elevators on this floor that might inter-
			//fere with the item. Also use the future tower decoration and bounds facilities to find
			//out where the floor ends so that people don't queue up in the air.
			recti rect = getFloorRect(qm->first);
			if (qp->first == kDown)
				rect.origin.x = rect.maxX();
			rect.size.x = 50;
			if (qp->first == kUp)
				rect.origin.x -= rect.size.x;
			
			//Use this rect
			qp->second->setRect(rect);
		}
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

void ElevatorItem::drawItem(rectd dirtyRect)
{
	TransportItem::drawItem(dirtyRect);
	
	//Draw the elevator cars
	drawCars(dirtyRect);
	
	//Draw the queues
	drawQueues(dirtyRect);
}

void ElevatorItem::drawBackground(rectd dirtyRect)
{
	TransportItem::drawBackground(dirtyRect);
	
	//Draw the motor and buffer
	motorSprite->draw();
	bufferSprite->draw();
	
	//Decide the lowest and highest visible floors.
	recti dirtyCells = tower->structure->worldToCell(dirtyRect);
	recti visibleCells = dirtyCells.intersectionRect(getRect());
	int lowestFloor = visibleCells.minY();
	int highestFloor = visibleCells.maxY() - 1;
	
	//Draw each visible floor.
	for (int i = lowestFloor; i <= highestFloor; i++)
		drawFloor(i);
}

void ElevatorItem::drawFloor(int f)
{
	//Calculate this floor's rect
	rectd rect = getFloorWorldRect(f);
	
	//Draw the floor background
	drawFloorBackground(f, rect);
	
	//Draw the floor number if the floor is active.
	if (isFloorActive(f)) {
		rectd numberRect = rect;
		numberRect.origin.x += 2;
		numberRect.size.x -= 2;
		drawFloorNumber(f, numberRect);
	}
}

void ElevatorItem::drawFloorBackground(int f, rectd rect)
{
	//Create a textured quad which we will use for drawing the background. We may also load the
	//background texture right at the beginning, since the different drawing steps all use areas
	//of the same background texture.
	TexturedQuad quad;
	quad.texture = backgroundNormal;
	
	//Draw the left wall.
	quad.rect = rect;
	quad.rect.size.x = 16;
	quad.textureRect.size.x = 16 / 352.0;
	quad.draw();
	
	//Draw the middle part
	quad.rect = rect.insetRect(int2(16, 0));
	quad.textureRect.origin.x = 10 / 352.0;
	quad.draw();
	
	//Draw the right wall
	quad.rect = rect;
	quad.rect.origin.x = quad.rect.maxX() - 16;
	quad.rect.size.x = 16;
	quad.textureRect.origin.x = quad.textureRect.size.x;
	quad.draw();
}

void ElevatorItem::drawFloorNumber(int f, rectd rect)
{
	//Create the textured quad we'll use for drawing the floor numbers.
	TexturedQuad quad;
	quad.rect = rect;
	
	//Decide what textures we're supposed to use
	bool highlighted = isFloorHighlighted(f);
	Texture * background = (highlighted ? backgroundHighlighted : backgroundNormal);
	Texture * ls = (highlighted ? lsHighlighted : lsNormal);
	Texture * ms = (highlighted ? msHighlighted : msNormal);
	
	//100th floor has its own bitmap
	if (f == 100) {
		quad.rect.size.x = 32;
		quad.rect.origin.x = rect.midX() - (quad.rect.size.x / 2);
		quad.texture = background;
		quad.textureRect = rectd(10.0 / 11, 0, 1.0 / 11, 1);
		quad.draw();
		return;
	}
	
	//Check whether the floor is negative
	bool negative = (f < 0);
	
	//Extract the MSD and LSD
	unsigned int msd = abs(f) / 10;
	unsigned int lsd = abs(f) % 10;
	
	//Calculate how many digits there will be
	unsigned int numberOfDigits = 1;
	if (msd > 0) numberOfDigits++;
	if (negative) numberOfDigits++;
	
	//Calculate the origin x coordinate
	quad.rect.origin.x = rect.midX() - (numberOfDigits * 12 / 2.0);
	
	//All digits share the same rect width
	quad.rect.size.x = 13;
	
	//Draw the B for negative floors if required
	if (negative) {
		//We need the ms texture for this
		quad.texture = ms;
		
		//Choose the texture rect which contains the B letter
		quad.textureRect.origin.x = 35.0 / ms->size.x;
		quad.textureRect.size.x = 13.0 / ms->size.x;
		
		//Draw the quad and advance its x coordinate for the next digit.
		quad.draw();
		quad.rect.origin.x += 12;
	}
	
	//The LS digits share the same texture and texture rect width
	quad.texture = ls;
	quad.textureRect.size.x = 13.0 / ls->size.x;
	
	//Draw the most significant digit, if there is one
	if (msd > 0) {
		//Choose the appropriate texture rect based on the digit value.
		quad.textureRect.origin.x = msd / 10.0;
		
		//Draw the quad and advance
		quad.draw();
		quad.rect.origin.x += 12;
		
		//Do some visual improvement. If the most significant digit was a 1, we may move the lsd a
		//bit closer.
		if (msd == 1)
			quad.rect.origin.x -= 2;
	}
	
	//Draw the least significant digit
	quad.textureRect.origin.x = lsd / 10.0;
	quad.draw();
}

void ElevatorItem::drawCars(rectd dirtyRect)
{
	for (CarSet::iterator it = cars.begin(); it != cars.end(); it++)
		(*it)->draw(dirtyRect);
}

void ElevatorItem::drawQueues(rectd dirtyRect)
{
	//Calculate the lowest and highest visible floors.
	recti dirtyCells = tower->structure->worldToCell(dirtyRect);
	int lowestFloor = dirtyCells.minY();
	int highestFloor = dirtyCells.maxY() - 1;
	
	//Iterate through all visible floors and each queue pair and ask them to draw.
	for (int i = lowestFloor; i <= highestFloor; i++)
		if (connectsToFloor(i))
			for (QueuePair::iterator qp = queues[i].begin(); qp != queues[i].end(); qp++)
				qp->second->draw(dirtyRect);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Events
//----------------------------------------------------------------------------------------------------

bool ElevatorItem::eventKeyDown(KeyEvent * event)
{
	ElevatorCar * car = *cars.begin();
	switch (event->unicode) {
		case '+':	debugFloor++; return true; break;
		case '-':	debugFloor--; return true; break;
		case 3:		car->setDestinationFloor(debugFloor); return true; break;
	}
	return false;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark People
//----------------------------------------------------------------------------------------------------

void ElevatorItem::didAddPerson(Person * person)
{
	if (!person)
		return;
	assert(connectsToFloor(person->getFloor()));
	
	//Decide which direction the person has to go
	Direction dir = (person->getStartFloor() < person->getEndFloor() ? kUp : kDown);
	
	//Add the person to the appropriate queue
	getQueue(person->getFloor(), dir)->addPerson(person);
}

void ElevatorItem::willRemovePerson(Person * person)
{
	//Remove the person from all queues (just to make sure).
	for (QueueMap::iterator qm = queues.begin(); qm != queues.end(); qm++)
		for (QueuePair::iterator qp = qm->second.begin(); qp != qm->second.end(); qp++)
			qp->second->removePerson(person);
	
	//And of course from all cars.
	//for (CarSet::iterator cs = cars.begin(); cs != cars.end(); cs++)
	//	(*cs)->removePerson(person);
}
