#pragma once
#include <map>
#include <string>
#include <tinyxml2.h>
#include <vector>
#include "../GameObject.h"
#include "Prototype.h"

namespace OT {

	/*
	 * The Game.cpp refers to the different objects
	 * by the icon number, so this is an enum to
	 * give readable names to the icon numbers.
	 */
	enum IconNumbers
	{
		ICON_LOBBY = 0,
		ICON_FLOOR = 1,
		ICON_STAIRS = 2,
		ICON_ELEVATOR = 3,
		ICON_OFFICE = 7,
		ICON_FASTFOOD = 11,
		ICON_RESTAURANT = 12,
		ICON_CINEMA = 14,
		ICON_PARTYHALL = 15,
		ICON_METRO = 19,
	};

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
