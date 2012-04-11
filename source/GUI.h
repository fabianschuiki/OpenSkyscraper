#pragma once

#include <Rocket/Core.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <string>

#include "Path.h"

namespace OT
{
	class GUIManager;
	
	class GUI
	{
	public:
		GUIManager * manager;
		Rocket::Core::Context * context;
		
		GUI(std::string name, GUIManager * manager);
		~GUI();
		
		bool handleEvent(sf::Event & event);
		void draw();
		
		Rocket::Core::ElementDocument * loadDocument(Path path);
	};
}