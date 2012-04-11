#pragma once
#include <Rocket/Core/ElementDocument.h>
#include <Rocket/Core/EventListener.h>
#include "GameObject.h"

namespace OT {
	
	class ToolboxWindow : public GameObject, private Rocket::Core::EventListener
	{
	public:
		ToolboxWindow(Game * game) : GameObject(game) {
			window          = NULL;
			speedButton     = NULL;
		}
		~ToolboxWindow() { close(); }
		
		void close();
		void reload();
		
		Rocket::Core::ElementDocument * window;
		Rocket::Core::Element * speedButton;
		typedef std::set<Rocket::Core::Element *> ElementSet;
		ElementSet buttons;
		
		void ProcessEvent(Rocket::Core::Event & event);
		
		void updateSpeed();
		
		std::string selectedTool;
		void selectTool(std::string tool);
	};
}
