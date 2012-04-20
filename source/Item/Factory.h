#pragma once
#include <map>
#include <string>
#include <tinyxml2.h>
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
			std::map<std::string, AbstractPrototype *> prototypesById;
			void loadPrototypes();
			
			Item * make(AbstractPrototype * prototype, int2 position = int2());
			Item * make(std::string prototypeID, int2 position = int2());
			Item * make(tinyxml2::XMLElement & xml);
		};
	}
}