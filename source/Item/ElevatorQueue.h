#pragma once
#include <list>
#include "../GameObject.h"
#include "../Math/Rect.h"

namespace OT {
	class Person;
	
	namespace Item {
		class Elevator;
		class ElevatorQueue : public GameObject
		{
		public:
			Elevator * const elevator;
			
			ElevatorQueue(Elevator * e);
			~ElevatorQueue();
			
			recti rect;
			bool called;
			double callTime;
			
			typedef std::list<Person *> People;
			People people;
			void add(Person * p);
			void remove(Person * p);
		};
	}
}
