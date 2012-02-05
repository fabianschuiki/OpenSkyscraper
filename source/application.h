#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "logger.h"
#include "path.h"

namespace OT
{
	class Application
	{
	public:
		Application(int argc, char * argv[]);
		
		Path getPath() const;
		Path getDir() const;
		
		Logger logger;
		
		sf::RenderWindow window;
		sf::VideoMode videoMode;
		
		int run();
		
	private:
		Path path;
		Path dir;
		void setPath(const Path & p);
		
		bool running;
		int exitCode;
		void init();
		void loop();
		void cleanup();
	};
	
	extern Application * App;
}
