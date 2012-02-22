#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <stack>

#include "DataManager.h"
#include "FontManager.h"
#include "GUIManager.h"
#include "Logger.h"
#include "Path.h"
#include "ResourceManager.h"
#include "SimTowerResources.h"
#include "State.h"

namespace OT
{
	class Application
	{
	public:
		Application(int argc, char * argv[]);
		
		Path getPath() const;
		Path getDataDir() const;
		Path getPrefsDir() const;
		
		Logger logger;
		
		sf::RenderWindow window;
		sf::VideoMode videoMode;
		
		DataManager data;
		GUIManager gui;
		ResourceManager<sf::Image> bitmaps;
		FontManager fonts;
		
		int run();
		
	private:
		Path path;
		Path dataDir;
		Path prefsDir;
		
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
