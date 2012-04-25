#include <cassert>
#include "../../Sprite.h"
#include "Elevator.h"
#include "Car.h"
#include "Queue.h"

using namespace OT;
using namespace OT::Item::Elevator;


Elevator::~Elevator()
{
	clearCars();
}

void Elevator::init()
{
	layer = 1;
	maxCarAcceleration = 7.5;
	maxCarSpeed = 10.0;
	maxCarCapacity = 21;
	
	Item::init();
	
	animation = 0;
	frame = 0;
	
	shaft.SetImage(app->bitmaps[shaftBitmap]);
	shaft.SetSubRect(sf::IntRect(0, 0, size.x*8, 36));
	shaft.SetCenter(0, 36);
	
	topMotor.SetImage(*shaft.GetImage());
	bottomMotor.SetImage(*shaft.GetImage());
	
	addSprite(&topMotor);
	addSprite(&bottomMotor);
	
	updateSprite();
	
	addCar(position.y);
}

void Elevator::updateSprite()
{
	int w = GetSize().x;
	int h = GetSize().y;
	
	topMotor.SetSubRect   (sf::IntRect((2*frame+1)*w, 0, (2*frame+2)*w, 36));
	bottomMotor.SetSubRect(sf::IntRect((2*frame+2)*w, 0, (2*frame+3)*w, 36));
	topMotor.SetCenter(0, 36);
	topMotor.SetY(-h);
}

void Elevator::Render(sf::RenderTarget & target) const
{
	Item::Render(target);
	
	//Draw the elevator floors.
	Sprite s = shaft;
	Sprite d;
	d.SetImage(app->bitmaps["simtower/elevator/digits"]);
	d.SetCenter(0, 17);
	
	int minY = 0;
	int maxY = size.y-1;
	
	for (int y = minY; y <= maxY; y++) {
		s.SetY(-y*36);
		d.SetY(-y*36 - 3);
		target.Draw(s);
		
		int flr = position.y + y;
		if (!connectsFloor(flr)) continue;
		
		char c[8];
		int len = snprintf(c, 8, "%i", flr);
		int x = 11 - (len - 1) * 6 + (size.x - 4) * 4;
		for (int i = 0; i < len; i++) {
			int p = 10;
			if (c[i] >= '0' && c[i] <= '9') p = c[i] - '0';
			d.SetSubRect(sf::IntRect(p*11, 0, (p+1)*11, 17));
			d.SetX(x);
			target.Draw(d);
			x += 12;
		}
	}
	
	//Draw the cars and queues.
	for (Cars::iterator c = cars.begin(); c != cars.end(); c++) target.Draw(**c);
	for (Queues::iterator q = queues.begin(); q != queues.end(); q++) target.Draw(**q);
}

void Elevator::advance(double dt)
{
	bool carsMoving = false;
	for (Cars::iterator ic = cars.begin(); ic != cars.end(); ic++) {
		(*ic)->advance(dt);
		if ((*ic)->state == Car::kMoving) carsMoving = true;
	}
	
	//Advance the queues so people get stressed.
	for (Queues::iterator iq = queues.begin(); iq != queues.end(); iq++) (*iq)->advance(dt);
	
	//Animate the elevator motors if there's a car moving.
	if (carsMoving) {
		animation = fmod(animation + dt, 1);
		int newFrame = floor(animation * 3);
		if (frame != newFrame) {
			frame = newFrame;
			updateSprite();
		}
	} else {
		animation = 0;
		if (frame != 0) {
			frame = 0;
			updateSprite();
		}
	}
}

void Elevator::encodeXML(tinyxml2::XMLPrinter & xml)
{
	Item::encodeXML(xml);
	xml.PushAttribute("height", size.y);
	for (std::set<int>::iterator i = unservicedFloors.begin(); i != unservicedFloors.end(); i++) {
		xml.OpenElement("unserviced");
		xml.PushAttribute("floor", *i);
		xml.CloseElement();
	}
	for (Cars::iterator c = cars.begin(); c != cars.end(); c++) {
		xml.OpenElement("car");
		(*c)->encodeXML(xml);
		xml.CloseElement();
	}
}

void Elevator::decodeXML(tinyxml2::XMLElement & xml)
{
	clearCars();
	Item::decodeXML(xml);
	size.y = xml.IntAttribute("height");
	for (tinyxml2::XMLElement * e = xml.FirstChildElement("unserviced"); e; e = e->NextSiblingElement("unserviced")) {
		unservicedFloors.insert(e->IntAttribute("floor"));
	}
	for (tinyxml2::XMLElement * e = xml.FirstChildElement("car"); e; e = e->NextSiblingElement("car")) {
		Car * car = new Car(this);
		car->decodeXML(*e);
		cars.insert(car);
	}
	updateSprite();
}

rectd Elevator::getMouseRegion()
{
	sf::Vector2f p = GetPosition();
	sf::Vector2f s = GetSize();
	return rectd(p.x, p.y - s.y - 36, s.x, s.y + 2*36);
}

void Elevator::repositionMotor(int motor, int y)
{
	assert(motor == -1 || motor == 1);
	int height;
	int newy;
	if (motor == -1) {
		newy = y + 1;
		height = (size.y + position.y - newy);
	} else {
		newy = position.y;
		height = (y - position.y);
	}
	if (height < 1)  height = 1;
	if (height > 30+1) height = 30+1;
	if (motor == -1) {
		newy = (size.y + position.y - height);
	}
	if (newy != position.y || height != size.y) {
		setPosition(int2(position.x, newy));
		size.y = height;
		//TODO: constrain cars to stay within elevator bounds.
		for (Cars::iterator c = cars.begin(); c != cars.end(); c++) (*c)->reposition();
		updateSprite();
		cleanQueues();
	}
}

void Elevator::clearCars()
{
	for (Cars::iterator c = cars.begin(); c != cars.end(); c++) delete *c;
	cars.clear();
}

void Elevator::addCar(int floor)
{
	Car * car = new Car(this);
	car->setAltitude(floor);
	cars.insert(car);
}

bool Elevator::connectsFloor(int floor) const
{
	if (floor < position.y || floor > position.y + size.y) return false;
	return !unservicedFloors.count(floor);
}

void Elevator::addPerson(Person * p)
{
	Item::addPerson(p);
	Direction dir = (p->journey.toFloor > p->journey.fromFloor ? kUp : kDown);
	getQueue(p->journey.fromFloor, dir)->addPerson(p);
}

void Elevator::removePerson(Person * p)
{
	for (Queues::iterator iq = queues.begin(); iq != queues.end(); iq++) {
		(*iq)->removePerson(p);
	}
	Item::removePerson(p);
}

/// Returns the queue for the given floor and direction, creating it if it does not yet exist.
Queue * Elevator::getQueue(int floor, Direction dir)
{
	for (Queues::iterator iq = queues.begin(); iq != queues.end(); iq++) {
		if ((*iq)->floor == floor && (*iq)->direction == dir) return *iq;
	}
	
	//Create the queue as there was none.
	Queue * q = new Queue(this);
	q->floor     = floor;
	q->direction = dir;
	q->width     = 400;
	queues.insert(q);
	return q;
}

/// Removes all queues on floors that the elevator doesn't connect to anymore. This is necessary
/// after the elevator motors are repositioned or certain floors deactivated.
void Elevator::cleanQueues()
{
	for (Queues::iterator iq = queues.begin(); iq != queues.end(); iq++) {
		if (!connectsFloor((*iq)->floor)) {
			for (int i = 0; i < 2; i++) delete *iq;
			queues.erase(iq);
		}
	}
}

/// Called whenever the first person queues up at an elevator queue. The elevator is now
/// responsible for making a car answer the call or postpone it.
void Elevator::called(Queue * queue)
{
	assert(queue);
	respondToCalls();
}

/// Responds to the most urgent calls.
void Elevator::respondToCalls()
{
	//Iterate through the queues, always getting the most urgent one.
	Queue * q;
	while (q = getMostUrgentQueue())
	{
		//Find the car best suited for responding. If none could be found, abort since all cars
		//are occupied.
		Car * car = getIdleCar(q->floor);
		if (!car) break;
		
		//Answer the call.
		q->answered = true;
		car->direction = q->direction;
		car->moveTo(q->floor);
	}
}

/// Returns the queue that is the most urgent to respond to.
Queue * Elevator::getMostUrgentQueue()
{
	Queue * queue = NULL;
	for (Queues::iterator iq = queues.begin(); iq != queues.end(); iq++) {
		if ((*iq)->called && !(*iq)->answered && (!queue || queue->getWaitDuration() < (*iq)->getWaitDuration())) queue = *iq;
	}
	return queue;
}

/// Returns the idle car closest to the given floor, or NULL if no car is idle.
Car * Elevator::getIdleCar(int floor)
{
	Car * car = NULL;
	for (Cars::iterator ic = cars.begin(); ic != cars.end(); ic++) {
		Car * c = *ic;
		if (c->state == Car::kIdle && (!car || fabs(car->altitude - floor) > fabs(c->altitude - floor))) car = c;
	}
	return car;
}

void Elevator::decideCarDestination(Car * car)
{
	//Find the next floor a passenger needs to go.
	int nextFloor = INT_MAX;
	for (People::iterator ip = car->passengers.begin(); ip != car->passengers.end(); ip++) {
		int f = (*ip)->journey.toFloor;
		if (nextFloor == INT_MAX || abs(car->destinationFloor - nextFloor) > abs(car->destinationFloor - f))
			nextFloor = f;
	}
	
	//Find the next queue that would lie in the car's path.
	Queue * nextQueue = NULL;
	double queueDistance = 0;
	for (Queues::iterator iq = queues.begin(); iq != queues.end(); iq++) {
		Queue * q = *iq;
		
		//Skip queues that aren't called, are answered, or are for the opposite direction.
		if (q->direction != car->direction) continue;
		if (!q->called || q->answered) continue;
		
		//Calculate the offset of the queue and the car, based on the car's current direction.
		//This will make queues that lie ahead of the car have a positive distance, and cars that
		//lie behin the car a negative distance. We only serve queues that are somewhat ahead of
		//the car.
		double distance = q->floor - car->altitude;
		distance *= car->direction;
		if (distance < 0.5) continue;
		
		//Keep the best queue.
		if (!nextQueue || queueDistance > distance) {
			queueDistance = distance;
			nextQueue = q;
		}
	}
	
	//Move the car to the closer of the two destinations.
	if (nextQueue && !car->isFull()) {
		double floorDistance = fabs(nextFloor - car->altitude);
		if (queueDistance <= floorDistance) {
			nextQueue->answered = true;
			car->moveTo(nextQueue->floor);
		}
	}
	getQueue(nextFloor, car->direction)->answered = true;
	car->moveTo(nextFloor);
}
