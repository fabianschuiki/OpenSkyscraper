#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "GUIManager.h"
#include "logger.h"
#include "path.h"

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
		
		GUIManager gui;
		
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
	};
	
	extern Application * App;
}
