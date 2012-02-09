#pragma once

#include <SFML/Window.hpp>

namespace OT
{
	/// Application uses State classes to draw stuff.
	class State
	{
	public:
		/// Called when the State becomes active, i.e. is the topmost State.
		virtual void activate() {}
		
		/// Called by the Application if an SFML event needs to be handled. Return true if the event was handled, false otherwise.
		virtual bool handleEvent(sf::Event & event) { return false; }
		
		/// All the gameloop work should be handled here. Simulate stuff, update state, draw things.
		virtual void advance(double dt) {}
		
		/// Called when the State becomes inactive, i.e. is not the topmost State anymore.
		virtual void deactivate() {}
	};
}
