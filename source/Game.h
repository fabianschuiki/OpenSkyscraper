#pragma once

#include "State.h"

namespace OT
{
	class Game : public State
	{
	public:
		void activate();
		void deactivate();
		
		bool handleEvent(sf::Event & event);
		void advance(double dt);
	};
}
