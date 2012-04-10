#pragma once

#include "Item/Factory.h"
#include "Item/Item.h"
#include "Sprite.h"
#include "State.h"

namespace OT {
	class Game : public State
	{
	public:
		Application & app;
		Game(Application & app);
		
		typedef std::set<Sprite *> Sprites;
		Sprites sprites;
		
		unsigned int drawnSprites;
		
		void activate();
		void deactivate();
		
		bool handleEvent(sf::Event & event);
		void advance(double dt);
		
		Item::Factory itemFactory;
		
		typedef std::set<Item::Item *> ItemSet;
		ItemSet items;
		void addItem(Item::Item * item);
		void removeItem(Item::Item * item);
		
	private:
		double zoom;
		sf::Vector2f poi;
		
		double skyFade;
		int    skyState;
		void drawBackground(const sf::FloatRect & rect);
		
		Rocket::Core::ElementDocument * timeWindow;
		Rocket::Core::ElementDocument * toolboxWindow;
		Rocket::Core::ElementDocument * mapWindow;
		
		void reloadGUI();
	};
}
