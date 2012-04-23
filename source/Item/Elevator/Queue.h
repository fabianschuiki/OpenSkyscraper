#pragma once
#include <list>
#include <SFML/Graphics/Drawable.hpp>
#include "../../GameObject.h"
#include "../../Math/Rect.h"

namespace OT {
	class Person;
	
	namespace Item {
		namespace Elevator {
			class Elevator;
			class Queue : public GameObject, public sf::Drawable
			{
			public:
				Elevator * const elevator;
				
				Queue(Elevator * e);
				~Queue();
				
				int floor;
				Elevator::Direction direction;
				int width;
				
				bool called;
				double callTime;
				
				typedef std::list<Person *> People;
				People people;
				void addPerson(Person * p);
				void removePerson(Person * p);
				
				virtual void advance(double dt);
				virtual void Render(sf::RenderTarget & target) const;
			};
		}
	}
}
