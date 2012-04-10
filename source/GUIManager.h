#pragma once

#include <Rocket/Core.h>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Rocket/Renderer.h"
#include "Rocket/SystemInterface.h"

namespace OT
{
	class GUIManager
	{
	public:
		Rocket::Core::Context * context;
		
		GUIManager();
		~GUIManager();
		
		bool init(sf::RenderWindow * window);
		bool handleEvent(sf::Event & event);
		void draw();
		
		Rocket::Core::ElementDocument * loadDocument(Path path);
		
	private:
		sf::RenderWindow * window;
		
		RocketRenderer renderer;
		RocketSystemInterface system;
	};
}
