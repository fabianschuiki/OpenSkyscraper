#pragma once

#include <tinyxml2.h>

#include "Item/Factory.h"
#include "Item/Item.h"
#include "Sky.h"
#include "Sprite.h"
#include "State.h"
#include "Time.h"
#include "TimeWindow.h"
#include "ToolboxWindow.h"

namespace OT {
	namespace Item { class AbstractPrototype; }
	
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
		
		int funds;
		int rating;
		void transferFunds(int f);
		void setFunds(int f);
		void setRating(int r);
		
		ToolboxWindow toolboxWindow;
		TimeWindow    timeWindow;
		
		Time time;
		
		bool paused;
		void setPaused(bool p);
		
		std::string selectedTool;
		int2 toolPosition;
		Item::AbstractPrototype * toolPrototype;
		Item::Item * itemBelowCursor;
		void selectTool(const char * tool);
		
		Sky sky;
		
	private:
		double zoom;
		double2 poi;
		
		Rocket::Core::ElementDocument * mapWindow;
		
		void reloadGUI();
		
		void encodeXML(tinyxml2::XMLPrinter & xml);
		void decodeXML(tinyxml2::XMLDocument & xml);
	};
}
