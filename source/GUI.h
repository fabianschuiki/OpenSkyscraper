#pragma once

#include "Path.h"

#include <Rocket/Core.h>
#include <SFML/Graphics/RenderWindow.hpp>

namespace OT
{
	class GUIManager;
	
	class GUI
	{
	public:
		GUIManager * manager;
		Rocket::Core::Context * context;
		
		GUI(GUIManager * manager);
		~GUI();
		
		bool handleEvent(sf::Event & event);
		void draw();
		
		Rocket::Core::ElementDocument * loadDocument(Path path);
	};
}