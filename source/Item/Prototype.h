#pragma once
#include <cstdio>
#include <string>
#include "../Math/Vector2D.h"
#include <tinyxml2.h>

namespace OT {
	class Game;
	
	namespace Item {
		class Factory;
		class Item;
		
		class AbstractPrototype
		{
		public:
			std::string id;
			std::string name;
			int price;
			int2 size;
			int icon;
			
			virtual Item * make(Game * game) = 0;
			
			std::string desc() {
				char c[512];
				snprintf(c, 512, "Prototype '%s' $%i %s", this->id.c_str(), this->price, this->size.desc().c_str());
				return c;
			}
		};

		template <typename T>
		class Prototype : public AbstractPrototype
		{
			friend class Factory;
		protected:
			Item * make(Game * game) { return new T(game, this); }
		};
	}
}