#pragma once
#include "../GameObject.h"
#include "Prototype.h"

namespace OT {
	namespace Item {
		
		class Item : public GameObject
		{
		public:
			AbstractPrototype * const prototype;
			Item(Game * game, AbstractPrototype * prototype) : GameObject(game), prototype(prototype) {}
		};
	}
}

#define OT_ITEM_CONSTRUCTOR(cls) cls(Game * game, AbstractPrototype * prototype) : Item(game, prototype)
