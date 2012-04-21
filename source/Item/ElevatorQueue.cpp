#include "../Application.h"
#include "../Game.h"
#include "../Person.h"
#include "Elevator.h"
#include "ElevatorQueue.h"

using namespace OT;
using namespace Item;


ElevatorQueue::ElevatorQueue(Elevator * e)
:	GameObject(e->game),
	elevator(e)
{
	called = false;
	callTime = 0;
}

ElevatorQueue::~ElevatorQueue()
{
	for (People::iterator ip = people.begin(); ip != people.end();) {
		LOG(DEBUG, "forcing passenger %p ahead", (*ip));
		(*(ip++))->journey.next();
	}
}

void ElevatorQueue::add(Person * p)
{
	people.push_back(p);
	if (!called) {
		called = true;
		callTime = game->time.absolute;
		//elevator->answerCalls();
	}
}

void ElevatorQueue::remove(Person * p)
{
	people.remove(p);
}
