#include <SFML/Graphics.hpp>

int main()
{
	//Create the main window.
	sf::RenderWindow app(sf::VideoMode(800, 600, 32), "OpenSkyscraper SFML");
	
	//Create a string to be drawn.
	sf::Font jura;
	jura.LoadFromFile("../Resources/fonts/Jura-Regular.ttf", 30);
	
	sf::String text("Welcome to OpenSkyscraper!", jura);
	text.SetColor(sf::Color(255, 255, 0));
	text.Move(100, 200);
	
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
		
		//Draw the text.
		app.Draw(text);
		
		//Display whatever we have drawn so far
		app.Display();
	}
	return 0;
}
