#pragma once
#include "Item.h"

namespace OT {
	namespace Item {
		
		class Stairlike : public Item
		{
		public:
			Stairlike(Game * game, OT::Item::AbstractPrototype * prototype) : Item(game, prototype) {}
			
			virtual void init();
			
			Sprite sprite;
			double animation;
			int frame;
			int frameCount;
			
			void updateSprite();
			virtual void advance(double dt);
			
			virtual bool canHaulPeople() const { return true; }
			virtual bool connectsFloor(int floor) const;
			virtual bool isStairlike() const { return true; }
			
			std::map<Person *, double> transitionTimes;
			virtual void addPerson(Person * p);
			virtual void removePerson(Person * p);
		};
	}
}
