#pragma once

#include "Sprite.h"
#include "State.h"

namespace OT
{
	class Game : public State
	{
	public:
		Application & app;
		Game(Application & app);
		
		typedef std::set<Sprite *> Sprites;
		Sprites sprites;
		
		void activate();
		void deactivate();
		
		bool handleEvent(sf::Event & event);
		void advance(double dt);
		
	private:
		double zoom;
		sf::Vector2f poi;
	};
}
