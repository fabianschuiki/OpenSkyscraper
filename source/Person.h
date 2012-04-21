#pragma once
#include <cstddef>
#include <queue>
#include "GameObject.h"
#include "Route.h"

namespace OT {
	namespace Item { class Item; }
	
	class Person : public GameObject
	{
	public:
		Person(Game * game);
		virtual ~Person();
		
		Item::Item * at;
		
		class Journey
		{
		public:
			Journey(Person * p) : person(p) {}
			
			void set(const Route & r);
			
			Item::Item * item();
			int toFloor();
			void next();
			
		private:
			Person * const person;
			std::queue<Route::Node> nodes;
		};
		Journey journey;
	};
}
