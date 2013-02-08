#include <cassert>
#include "../../Game.h"
#include "Car.h"
#include "Queue.h"

using namespace OT;
using namespace OT::Item::Elevator;

const static double kDoorPeriod    = 0.1;  //seconds it takes the door to open/close
const static double kWaitTime      = 0.15; //seconds the elevator waits before closing doors
const static double kMountPeriod   = 0.03; //seconds it takes for one person to get on the elevator
const static double kUnmountPeriod = 0.03; //dito, but off the elevator


void Car::init()
{
	sprite.SetImage(app->bitmaps[elevator->carBitmap]);
	sprite.SetCenter(0, 30);
	updateSprite();
	
	arrivingSound.SetBuffer(app->sounds["simtower/elevator/arriving"]);
	departingSound.SetBuffer(app->sounds["simtower/elevator/departing"]);
	arrivingPlayed = false;
	departingPlayed = false;
	
	direction = Elevator::kNone;
	startAltitude = altitude;
	destinationFloor = altitude;
	journeyTime = 0;
	state = kIdle;
}

void Car::setAltitude(double a)
{
	if (altitude != a) {
		altitude = a;
		reposition();
	}
}

void Car::reposition()
{
	SetPosition(0, -altitude * 36 - elevator->GetPosition().y);
}

void Car::updateSprite()
{
	int index = 3;
	int pc = passengers.size();
	if (pc == 0) index = 0;
	else if (pc <= 1) index = 1;
	else if (pc <= 3) index = 2;
	else if (pc == elevator->maxCarCapacity) index = 4;
	
	int w = sprite.GetImage()->GetWidth() / 5;
	int h = sprite.GetImage()->GetHeight();
	sprite.SetSubRect(sf::IntRect(index*w, 0, (index+1)*w, h));
	sprite.SetPosition(sf::Vector2f(2, 0));
}

void Car::Render(sf::RenderTarget & target) const
{
	target.Draw(sprite);
}

void Car::encodeXML(tinyxml2::XMLPrinter& xml)
{
	xml.PushAttribute("altitude", altitude);
}

void Car::decodeXML(tinyxml2::XMLElement& xml)
{
	setAltitude(xml.DoubleAttribute("altitude"));
	startAltitude = altitude;
	destinationFloor = altitude;
}

void Car::setState(State s)
{
	if (state != s) {
		state = s;
		journeyTime = 0;
		if (state == kIdle) {
			direction = Elevator::kNone;
			elevator->respondToCalls();
		}
	}
}

void Car::advance(double dt)
{
	int passengersBefore = passengers.size();
	
	//Advance the journey time.
	journeyTime += game->time.dta / Time::kBaseSpeed;
	
	//If we're not on our destination floor, we have to move there. Note that due to float/double
	//imprecision, we can't use a simple comparison, but rather have to check whether the distance
	//between destination floor and current floor is above a certain treshold which we consider "at
	//destination".
	if (fabs(destinationFloor - altitude) > 0.01) {
		//First we need two constants. The acceleration of the elevator and the absolute maximum
		//speed.
		double a = elevator->maxCarAcceleration;
		double vmax = elevator->maxCarSpeed;
		
		//Later we'll also need the distance between the start and destination.
		double s = fabs(destinationFloor - startAltitude);
		
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
		double v = std::min<double>(vmax, sqrt(2 * q * a * s));
		
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
		if (destinationFloor > startAltitude)
			setAltitude(startAltitude + d);
		else
			setAltitude(startAltitude - d);
		
		//As a security measure, we use t1 to make sure we stop animating the elevator at some
		//point. Just in case the algorithm blows up, which it shouldn't, but well...
		if (journeyTime > t1)
			setAltitude(destinationFloor);
		
		//Now we also want to play some sounds. If the travel is long enough, we play the departing
		//sound.
		if (!departingPlayed) {
			if (t1 >= 1) departingSound.Play(game);
				//Audio::shared()->play(departingSound);
			departingPlayed = true;
		}
		
		//And as we approach the destination floor we also want the sound to be played.
		if (!arrivingPlayed && (s - d) < 0.1) {
			arrivingSound.Play(game);
			//Audio::shared()->play(arrivingSound);
			arrivingPlayed = true;
		}
		
		//We should probably show the inner workings of the algorithm in the console for debuggin.
		//DEBUG: remove this log stuff later. Seems to work anyway.
		char buffer[512];
		snprintf(buffer, 512, "s=%3.0f, v=%2.1f, t0=%2.1f, t1=%2.1f, tacc=%2.1f, tdec=%2.1f, %s",
		s, v, t0, t1, tacc, tdec,
		(phase == 0 ? "accelerating" : (phase == 1 ? "travelling" : "decelerating")));
		//LOG(DEBUG, buffer);
	}
	
	else {
		//Just to make sure :)
		setAltitude(destinationFloor);
		
		//So we're obviously on the destination floor. Quite a few things to do here. First we have
		//to act according to our current state. So let's do that.
		//TODO: This needs some heavy documentation!
		switch (state) {
			
			case kMoving: {
				if (direction != Elevator::kNone)
					setState(kOpeningDoors);
				else
					setState(kIdle);
			} break;
			
			case kOpeningDoors: {
				if (journeyTime >= kDoorPeriod) setState(kHauling);
			} break;
			
			case kHauling: {
				Person * p = NULL;
				bool handled = false;
				assert(direction != Elevator::kNone);
				
				//Fetch the queue we're serving
				Queue * q = elevator->getQueue(destinationFloor, direction);
				
				//Unhaul
				if (!handled && nextPassengerToUnmount()) {
					while (journeyTime >= kUnmountPeriod && (p = nextPassengerToUnmount())) {
						journeyTime -= kUnmountPeriod;
						passengers.erase(p);
						p->journey.next();
					}
					handled = true;
				}
				
				//Haul
				if (!handled && !isFull() && q && !q->people.empty()) {
					q->steppingInside = true;
					while (journeyTime >= kMountPeriod && !isFull() && (p = q->popPerson())) {
						journeyTime -= kMountPeriod;
						passengers.insert(p);
					}
					handled = true;
				}
				
				//Hauling complete, decide what to do.
				//TODO: make this time constant adjustable!
				if (!handled && (journeyTime >= kWaitTime || isFull())) {
					if (q) {
						q->steppingInside = false;
						q->called = false;
					}
					q->answered = false;
					if (!passengers.empty())
						setState(kClosingDoors);
					else
						setState(kIdle);
				}
			} break;
			
			case kClosingDoors: {
				if (journeyTime >= kDoorPeriod) {
					assert(!passengers.empty());
					elevator->decideCarDestination(this);
				}
			} break;
		}
	}
	
	//Update the car sprite if the number of passengers changed.
	if (passengersBefore != passengers.size())
		updateSprite();
}

bool Car::isFull()
{
	return (passengers.size() >= elevator->maxCarCapacity);
}

Person * Car::nextPassengerToUnmount()
{
	//Iterate through all passengers and return the first one we find that has the current floor
	//as its end floor, i.e. that needs to get off on this floor.
	for (Item::People::iterator ip = passengers.begin(); ip != passengers.end(); ip++)
		if ((*ip)->journey.toFloor == destinationFloor)
			return *ip;
	return NULL;
}

void Car::moveTo(int floor)
{
	if (destinationFloor != floor) {
		destinationFloor = floor;
		
		arrivingPlayed = false;
		departingPlayed = false;
		
		startAltitude = altitude;
		setState(kMoving);
	} else {
		setState(kHauling);
	}
}

/** Removes the given passenger from this car. Does nothing if the person is not in the car. */
void Car::removePassenger(Person *p)
{
	passengers.erase(p);
}
