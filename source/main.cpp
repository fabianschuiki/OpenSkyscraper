#include <iostream>
#include <SFML/Graphics.hpp>
#include "space.h"
#include "spaceslice.h"
#include "sprite.h"

int main()
{
	//Create the main window.
	sf::RenderWindow app(sf::VideoMode(800, 600, 32), "OpenSkyscraper SFML");
	
	//Create a string to be drawn.
	sf::Font jura;
	jura.LoadFromFile("../Resources/fonts/Jura-Regular.ttf", 16);
	
	sf::String text(L"Welcome to OpenSkyscraper!", jura, 16);
	text.SetColor(sf::Color(255, 255, 0));
	text.Move(0, 0);

	//Load an image to be drawn.
	sf::Image condo;
	condo.LoadFromFile("../Resources/condo.png");
	condo.SetSmooth(false);
	
	//Create a sprite that uses the image.
	Sprite sprite;
	sprite.SetImage(condo);
	sprite.SetPosition(80, 70);
	
	//Create some space partition.
	Space s;
	s.addSprite(&sprite);
	
	//Center the view.
	app.GetDefaultView().SetCenter(0, 0);
	
	//Get the space slice.
	SpaceSlice * slice = NULL;
	
	//Run the main loop.
	while (app.IsOpened()) {
		
		//Process events.
		sf::Event event;
		while (app.GetEvent(event)) {
			
			//Close window.
			if (event.Type == sf::Event::Closed)
				app.Close();
			
			//Window resized.
			if (event.Type == sf::Event::Resized) {
				std::cout << "resized to " << event.Size.Width << " x " << event.Size.Height << "\n";
				app.GetDefaultView().SetHalfSize(app.GetWidth()/2.0, app.GetHeight()/2.0);
				app.GetDefaultView().Zoom(std::min<double>(1, std::min<double>(app.GetWidth() / 800.0, app.GetHeight() / 600.0)));
				if (slice) delete slice;
				slice = NULL;
			}
			
			//Text entered.
			if (event.Type == sf::Event::KeyPressed) {
				std::cout << "key pressed " << event.Key.Code << "\n";
				if (event.Key.Code == sf::Key::Back) {
					std::wstring t = text.GetText();
					std::wstring n(t.begin(), t.end() - 2);
					text.SetText(n);
				}
			}
			if (event.Type == sf::Event::TextEntered) {
				text.SetText((std::wstring)text.GetText() + (wchar_t)event.Text.Unicode);
			}
			
			//Sprite movement.
			if (event.Type == sf::Event::KeyPressed) {
				switch (event.Key.Code) {
					case sf::Key::Left:		app.GetDefaultView().Move(-10, 0); break;
					case sf::Key::Right:	app.GetDefaultView().Move( 10, 0); break;
					case sf::Key::Up:		app.GetDefaultView().Move(0, -10); break;
					case sf::Key::Down:		app.GetDefaultView().Move(0,  10); break;
				}
				if (slice) delete slice;
				slice = NULL;
			}
		}
		
		//Update the slice if required.
		if (!slice) {
			const sf::FloatRect & rect = app.GetDefaultView().GetRect();
			slice = s.makeSlice(rect.Left + 200, rect.Top, rect.Right - 200, rect.Bottom);
			std::cout << "slice contains " << slice->partitions.size() << " partitions\n";
		}
		
		//Clear the screen.
		app.Clear();
		
		//Draw the text.
		app.Draw(text);
		
		//Draw the sprites.
		app.Draw(sprite);
		
		//Draw other stuff.
		s.draw(app);
		
		//Display whatever we have drawn so far
		app.Display();
	}
	return 0;
}
