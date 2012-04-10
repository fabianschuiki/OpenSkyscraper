#pragma once
#include <vector>
#include "../GameObject.h"
#include "Prototype.h"

namespace OT {
	namespace Item {
		
		class Factory : public GameObject
		{
		public:
			Factory(Game * game) : GameObject(game) {}
			~Factory();
			
			std::vector<AbstractPrototype *> prototypes;
			void loadPrototypes();
		};
	}
}