#include <SFML/Graphics.hpp>

int main()
{
	//Create the main window.
	sf::RenderWindow app(sf::VideoMode(800, 600, 32), "OpenSkyscraper SFML");
	
	//Run the main loop.
	while (app.IsOpened()) {
		
		//Process events.
		sf::Event event;
		while (app.GetEvent(event)) {
			
			//Close window.
			if (event.Type == sf::Event::Closed) app.Close();
		}
		
		//Clear the screen.
		app.Clear();
		
		//Display whatever we have drawn so far
		app.Display();
	}
	return 0;
}
