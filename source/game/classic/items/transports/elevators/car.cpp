#include "car.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

ElevatorCar::ElevatorCar(ElevatorItem * elevator) : elevator(elevator),
updateTextureIfNeeded(this, &ElevatorCar::updateTexture, &updateIfNeeded),
updatePositionIfNeeded(this, &ElevatorCar::updatePosition, &updateIfNeeded)
{
	//Initialize the sprite
	sprite = new Sprite;
	
	//Initialize the arriving sound effect
	arrivingSound = new SoundEffect;
	arrivingSound->sound = Sound::named("simtower/transport/elevator/arriving");
	arrivingSound->layer = SoundEffect::kTopLayer;
	arrivingSound->copyBeforeUse = true;
	
	//Initialize the departing sound effect
	departingSound = new SoundEffect;
	departingSound->sound = Sound::named("simtower/transport/elevator/departing");
	departingSound->layer = SoundEffect::kTopLayer;
	departingSound->copyBeforeUse = true;
	
	//Initialize the basic values
	floor = 0;
	direction = ElevatorItem::kNone;
	
	startFloor = floor;
	destinationFloor = floor;
	journeyTime = 0;
	
	arrivingPlayed = false;
	departingPlayed = false;
	
	state = kIdle;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark People
//----------------------------------------------------------------------------------------------------

void ElevatorCar::addPassenger(Person * p)
{
	if (!p) return;
	passengers.insert(p);
	updateTextureIfNeeded.setNeeded();
}

void ElevatorCar::removePassenger(Person * p)
{
	if (!p) return;
	passengers.erase(p);
	updateTextureIfNeeded.setNeeded();
}

bool ElevatorCar::hasPassengers()
{
	return !passengers.empty();
}

bool ElevatorCar::isFull()
{
	return (passengers.size() >= elevator->maxCarCapacity());
}



Person * ElevatorCar::nextPassengerToUnmount()
{
	//Iterate through all passengers and return the first one we find that has the current floor
	//as its end floor, i.e. that needs to get off on this floor.
	for (PersonSet::iterator it = passengers.begin(); it != passengers.end(); it++)
		if ((*it)->getEndFloor() == getDestinationFloor())
			return (*it);
	return NULL;
}

int ElevatorCar::closestPassengerDestinationFloor()
{
	//Iterate through all passengers and find the destination floor closest to the current one.
	bool valid = false;
	int dst = getDestinationFloor();
	int closest = dst;
	for (PersonSet::iterator it = passengers.begin(); it != passengers.end(); it++) {
		int f = (*it)->getEndFloor();
		if (!valid || abs(dst - closest) > abs(dst - f)) {
			closest = f;
			valid = true;
		}
	}
	
	//Return whatever we've found
	return closest;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

double ElevatorCar::getFloor()
{
	return floor;
}

void ElevatorCar::setFloor(double f)
{
	if (floor != f) {
		floor = f;
		updatePositionIfNeeded.setNeeded();
	}
}



ElevatorItem::Direction ElevatorCar::getDirection()
{
	return direction;
}

void ElevatorCar::setDirection(ElevatorItem::Direction dir)
{
	if (direction != dir) {
		direction = dir;
	}
}



int ElevatorCar::getDestinationFloor()
{
	return destinationFloor;
}

void ElevatorCar::setDestinationFloor(int f)
{
	if (destinationFloor != f) {
		destinationFloor = f;
		
		//Reset the animation
		startFloor = getFloor();
		journeyTime = 0;
		
		//Reset the audio state
		arrivingPlayed = false;
		departingPlayed = false;
		
		//Mark us as moving around
		setState(kMoving);
	} else {
		//Obviously we're already on the floor where we'd like to be, so we might as well just
		//start hauling right away.
		setState(kHauling);
	}
}



ElevatorCar::State ElevatorCar::getState()
{
	return state;
}

void ElevatorCar::setState(State s)
{
	if (state != s) {
		state = s;
		journeyTime = 0;
		
		const static string states[5] = {
			"idle", "moving", "opening doors", "hauling", "closing doors"
		};
		OSSObjectLog << states[s] << std::endl;
		
		//If we just switched to idle state we have to inform our elevator about this so it may
		//assign us a new call to respond to. We also have to reset our direction.
		if (state == kIdle) {
			setDirection(ElevatorItem::kNone);
			elevator->respondToCallsIfNeeded.setNeeded();
		}
	}
}

bool ElevatorCar::isIdle()
{
	//WTF: long bughunting ended in this line being state != kIdle. What was I thinking?
	return (state == kIdle);
}



void ElevatorCar::advance(double dt)
{
	//Advance the journey time.
	journeyTime += dt;
	
	//If we're not on our destination floor, we have to move there. Note that due to float/double
	//imprecision, we can't use a simple comparison, but rather have to check whether the distance
	//between destination floor and current floor is above a certain treshold which we consider "at
	//destination".
	if (fabs(destinationFloor - floor) > 0.01) {
		//First we need two constants. The acceleration of the elevator and the absolute maximum
		//speed.
		double a = elevator->maxCarAcceleration();
		double vmax = elevator->maxCarSpeed();
		
		//Later we'll also need the distance between the start and destination.
		double s = fabs(destinationFloor - startFloor);
		
		//Now we have to calculate the speed we're able to achieve on this journey. This calculation
		//uses the constant q which describes the fraction of the distance that should be used for
		//acceleration. Setting it to 1/3 makes the elevator accelerate in the first third, travel
		//at speed in the second third, and decelerate in the last third. This values has great im-
		//pact on short distance travels where the elevator is not able to reach maximum speed. A
		//value of 1/2 would allow the elevator to accelerate right until it is as far from the des-
		//tination as it is from the origin; but that looks ugly. A small value like 1/8 results in
		//extremely slow short distance travelling since the elevator has almost no chance to gain
		//speed since it's only allowed to do so in one eighth of the distance.
		double q = 1.0 / 3;
		double v = mind(vmax, sqrt(2 * q * a * s));
		
		//The next thing we need are two time values. They are somewhat difficult to explain since
		//they are given rise by the formulae used to calculate the speed. The second constant, t1,
		//is easily explained. It is the time from the start of the journey at which the elevator
		//comes to a full stop at the destination floor. The first one, t0, is difficult to explain.
		//It describes the time offset between two cars, one accelerated at a and one at full speed
		//right from the start, so that the accelerated car when it reaches maximum speed is at the
		//same location as the other car.
		double t0 = v / (2 * a);
		double t1 = (s / v) + 2 * t0;
		
		//Now we need two points in time in order to calculate the car's position appropriately.
		//The first one, tacc, describes the time it takes the car to accelerate to its travel
		//speed. The second, tdec, describes the time at which the elevator has to start decele-
		//rating in order to stop at the destination floor.
		double tacc = v / a;
		double tdec = t1 - tacc;
		
		//Decide what phase the car is in. There are three phases: acceleration, travel, decele-
		//ration. Before tacc, the car is still accelerating. Between tacc and tdec, the car is
		//traveling at speed v. After tdec the car is decelerating until it reaches the destination
		//floor.
		unsigned int phase = 0;
		if (journeyTime > tacc) phase = 1;
		if (journeyTime > tdec) phase = 2;
		
		//Abbreviate the journeyTime so we don't have that many letters in our formulae.
		double t = journeyTime;
		
		//Calculate the distance from the start based on the phase the car is in. The easiest way
		//to understand the three equations is to hack them into a graph calculator or some plotting
		//software on your computer. This gives you a feel of how they work. The basic idea is that
		//all three of them describe the motion of the car in a certain time period accurately. They
		//are crafted so that the phase functions 0 and 1 touch at tacc, and 1 and 2 touch at tdec.
		//Or in other words, phase 1 is tangent of phase 0 at t=tacc and phase 2 at t=tdec.
		double d;
		switch (phase) {
				//Acceleration. This the equation for uniformly accelerated motion from physics 101.
			case 0: d = 0.5 * a * (t*t); break;
				
				//Travel. This is the even simpler equation for motion at constant velocity. The
				//only quirk is the time offset t0 we subtract from t.
			case 1: d = v * (t - t0); break;
				
				//Deceleration. Basically the same as the acceleration. Only do we set s as the tar-
				//get distance and calculate the deceleration backwards in time t is subtracted from
				//t1, which gives a movement back in time.
			case 2: d = s - (0.5 * a * (t1 - t)*(t1 - t)); break;
		}
		
		//Now we know the distance from the starting point based on the time. All we have to do now
		//is calculate the actual floor we're on.
		if (destinationFloor > startFloor)
			setFloor(startFloor + d);
		else
			setFloor(startFloor - d);
		
		//As a security measure, we use t1 to make sure we stop animating the elevator at some
		//point. Just in case the algorithm blows up, which it shouldn't, but well...
		if (journeyTime > t1)
			setFloor(destinationFloor);
		
		//Now we also want to play some sounds. If the travel is long enough, we play the departing
		//sound.
		if (!departingPlayed) {
			if (t1 >= 1)
				Audio::shared()->play(departingSound);
			departingPlayed = true;
		}
		
		//And as we approach the destination floor we also want the sound to be played.
		if (!arrivingPlayed && (s - d) < 0.1) {
			Audio::shared()->play(arrivingSound);
			arrivingPlayed = true;
		}
		
		//We should probably show the inner workings of the algorithm in the console for debuggin.
		//DEBUG: remove this log stuff later. Seems to work anyway.
		/*char buffer[512];
		snprintf(buffer, 512, "s=%3.0f, v=%2.1f, t0=%2.1f, t1=%2.1f, tacc=%2.1f, tdec=%2.1f, %s",
				 s, v, t0, t1, tacc, tdec,
				 (phase == 0 ? "accelerating" : (phase == 1 ? "travelling" : "decelerating")));
		std::cout << buffer << std::endl;*/
		
	} else {
		
		//Just to make sure :)
		setFloor(destinationFloor);
		
		//So we're obviously on the destination floor. Quite a few things to do here. First we have
		//to act according to our current state. So let's do that.
		switch (getState()) {
				
			case kMoving: {
				if (getDirection() != ElevatorItem::kNone)
					setState(kOpeningDoors);
				else
					setState(kIdle);
			} break;
				
			case kOpeningDoors: {
				if (journeyTime >= 0.1)
					setState(kHauling);
			} break;
				
			case kHauling: {
				Person * p = NULL;
				assert(getDirection() != ElevatorItem::kNone);
				
				//Fetch the queue we're serving
				ElevatorQueue * q = elevator->getQueue(getDestinationFloor(), getDirection());
				
				//Unhaul
				if (!p) {
					while (journeyTime >= 0.05 && (p = nextPassengerToUnmount())) {
						journeyTime -= 0.05;
						removePassenger(p);
						p->setFloor(getDestinationFloor());
					}
				}
				
				//Haul
				if (!p && q) {
					q->setSteppingInside(true);
					while (journeyTime >= 0.05 && !isFull() && (p = q->popPerson())) {
						journeyTime -= 0.05;
						addPassenger(p);
					}
				}
				
				//Hauling complete, decide what to do.
				if (!p && journeyTime >= 0.25) {
					if (q) {
						q->setSteppingInside(false);
						q->clearCall();
					}
					if (hasPassengers())
						setState(kClosingDoors);
					else
						setState(kIdle);
				}
			} break;
				
			case kClosingDoors: {
				if (journeyTime >= 0.1)
					decideDestination();
			} break;
		}
	}
}



void ElevatorCar::decideDestination()
{
	//If we don't have any passengers switch to idle state and abort. This check has usually already
	//been done by advance(), but just in case to make sure we're not deciding something stupid.
	if (!hasPassengers()) {
		setState(kIdle);
		return;
	}
	
	//First we ask our passengers about the next floor they want to go.
	int passengerWish = closestPassengerDestinationFloor();
	
	//Now we ask the elevator where the next call to be answered would be, based on our direction.
	ElevatorQueue * nextQueue = elevator->getNextQueue(this);
	
	//If we're closer to the next call than the passenger wish and aren't full (quite important,
	//acutally ^^), we answer the call first. We do this by first calculating both absolute dist-
	//ances.
	if (nextQueue && !isFull()) {
		double passengerDistance = fabs(passengerWish - getFloor());
		double queueDistance = fabs(nextQueue->getRect().minY() - getFloor());
		if (queueDistance <= passengerDistance) {
			answerCall(nextQueue);
			return;
		}
	}
	
	//Service the passenger since there was obviously no other call to handle first.
	setDestinationFloor(passengerWish);
}

void ElevatorCar::answerCall(ElevatorQueue * q)
{
	if (!q) return;
	setDirection(q->getDirection());
	setDestinationFloor(q->getRect().minY());
	q->answerCall(this);
	OSSObjectLog << q->description() << std::endl;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark State
//----------------------------------------------------------------------------------------------------

unsigned int ElevatorCar::getTextureSliceIndex()
{
	unsigned int c = passengers.size();
	double f = (double)c / elevator->maxCarCapacity();
	
	if (c == 0)
		return 0;
	if (c == 1)
		return 1;
	if (f < 0.25)
		return 2;
	if (!isFull())
		return 3;
	return 4;
}

void ElevatorCar::update()
{
	//Update the texture and position if required
	updateTextureIfNeeded();
	updatePositionIfNeeded();
}

void ElevatorCar::updateTexture()
{
	//Get the texture slice to use as the car texture (empty, 1, 3, many or full)
	unsigned int slice = getTextureSliceIndex();
	
	//Load the appropriate car texture
	stringstream textureName;
	textureName << "simtower/transport/elevator/";
	textureName << elevator->getTypeName();
	textureName << "/car/";
	textureName << (slice == 0 ? "empty" : "occupied");
	sprite->texture = Texture::named(textureName.str());
	
	//Choose the appropriate slice of the texture
	if (slice == 0) {
		sprite->textureRect = rectd(0, 0, 1, 1);
	} else {
		sprite->textureRect = rectd(0, 0, 0.25, 1);
		sprite->textureRect.origin.x = (slice - 1) * 0.25;
	}
	
	//Inset the texture rect somewhat so that we only get the elevator car, not the background too.
	sprite->textureRect.origin += double2(2, 1) / sprite->texture->size;
	sprite->textureRect.size -= double2(4, 6) / sprite->texture->size;
	
	//Set the car rect size
	sprite->rect.size.x = elevator->getWorldRect().size.x - 4;
	sprite->rect.size.y = 30;
}

void ElevatorCar::updatePosition()
{
	sprite->rect.origin.x = elevator->getWorldRect().origin.x;
	sprite->rect.origin.y = /*round(*/getFloor() * elevator->tower->structure->cellSize.y/*)*/;
	sprite->rect.origin += int2(2, 1);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

void ElevatorCar::draw(rectd dirtyRect)
{
	sprite->draw();
}
