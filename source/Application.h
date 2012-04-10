#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <stack>

#include "BitmapManager.h"
#include "DataManager.h"
#include "FontManager.h"
#include "GUIManager.h"
#include "Logger.h"
#include "Path.h"
#include "State.h"

namespace OT
{
	class Application
	{
	public:
		Application(int argc, char * argv[]);
		
		Path getPath() const { return path; }
		
		Logger logger;
		
		sf::RenderWindow window;
		sf::VideoMode videoMode;
		
		DataManager   data;
		GUIManager    gui;
		BitmapManager bitmaps;
		FontManager   fonts;
		
		int run();
		
	private:
		Path path;
		void setPath(const Path & p);
		
		bool running;
		int exitCode;
		
		void init();
		void loop();
		void cleanup();
		
		std::stack<State *> states;
		void pushState(State * state);
		void popState();
	};
	
	extern Application * App;
}
