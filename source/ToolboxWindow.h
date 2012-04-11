#pragma once
#include <Rocket/Core/ElementDocument.h>
#include "GameObject.h"

namespace OT {
	
	class ToolboxWindow : public GameObject
	{
	public:
		ToolboxWindow(Game * game) : GameObject(game) { window = NULL; }
		~ToolboxWindow() { close(); }
		
		void close();
		void reload();
		
		Rocket::Core::ElementDocument * window;
		std::vector<Rocket::Core::Element *> buttons;
	};
}
