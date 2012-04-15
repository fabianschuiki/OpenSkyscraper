#include "ElevatorQueue.h"

using namespace OT;
using namespace Item;


ElevatorQueue::ElevatorQueue(Elevator * elevator)
:	GameObject(elevator->game),
	elevator(elevator)
{
}
