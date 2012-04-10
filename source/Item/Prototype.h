#pragma once
#include <cstdio>
#include <string>
#include "../Math/Vector2D.h"

namespace OT {
	class Game;
	
	namespace Item {
		class Item;
		
		class AbstractPrototype
		{
		public:
			std::string name;
			int price;
			int2 size;
			int2 icon;
			
			virtual Item * make(Game * game) = 0;
			
			std::string desc() {
				char c[512];
				snprintf(c, 512, "Prototype '%s' $%i %s", this->name.c_str(), this->price, this->size.desc().c_str());
				return c;
			}
		};

		template <typename T>
		class Prototype : public AbstractPrototype
		{
		public:
			Item * make(Game * game) { return new T(game, this); }
		};
	}
}