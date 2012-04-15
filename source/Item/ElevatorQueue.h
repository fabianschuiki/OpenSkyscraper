#pragma once
#include "Elevator.h"

namespace OT {
	namespace Item {
		class ElevatorQueue : public GameObject
		{
		public:
			Elevator * const elevator;
			ElevatorQueue(Elevator * elevator);
			
			recti rect;
			Elevator::Direction direction;
			
			typedef std::list<Person *> People;
			People people;
			void addPerson(Person * p);
			void removePerson(Person * p);
		};
	}
}
