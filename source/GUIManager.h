#pragma once

#include <Rocket/Core.h>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Rocket/Renderer.h"
#include "Rocket/SystemInterface.h"

namespace OT
{
	class GUI;
	
	class GUIManager
	{
		friend class GUI;
		
	public:
		GUIManager();
		~GUIManager();
		
		bool init(sf::RenderWindow * window);
		
		Rocket::Core::Input::KeyIdentifier translateKey(sf::Keyboard::Key key);
		int getKeyModifiers();
		
	protected:
		sf::RenderWindow * window;
		
	private:
		RocketRenderer renderer;
		RocketSystemInterface system;
	};
}
