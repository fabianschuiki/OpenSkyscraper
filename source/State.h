#pragma once

#include <SFML/Window.hpp>
#include <Rocket/Core/ElementDocument.h>

namespace OT
{
	class Application;
	
	/// Application uses State classes to draw stuff.
	class State
	{
	public:
		char debugString[512];
		State();
		
		virtual void activate();
		virtual bool handleEvent(sf::Event & event) { return false; }
		virtual void advance(double dt) {}
		virtual void deactivate();
		
		bool isActive() { return active; }
		
		Rocket::Core::ElementDocument * getGUI() { return gui; }
		void setGUI(Rocket::Core::ElementDocument * gui);
		
	private:
		bool active;
		Rocket::Core::ElementDocument * gui;
	};
}
