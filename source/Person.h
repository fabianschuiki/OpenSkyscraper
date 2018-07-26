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
		typedef enum {
			kMan = 0,
			kSalesman,
			kWoman1,
			kChild,
			kWoman2,
			kHousekeeper,
			kWomanWithChild1,
			kWomanWithChild2,
			kSecurity
		} Type;

		Person(Game * game, Type type = kMan);
		virtual ~Person();
		
		Item::Item * at;
		
		class Journey
		{
		public:
			Journey(Person * p);
			
			void set(const Route & r);
			
			void next();
			int fromFloor;
			Item::Item * item;
			int toFloor;
			
		private:
			Person * const person;
			std::queue<Route::Node> nodes;
		};
		Journey journey;
		Type type;
		double stress;
		int getWidth();
	};
}
