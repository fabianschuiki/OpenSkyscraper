#include <iostream>
#include <SFML/Graphics.hpp>

#include "Application.h"
#include "space.h"
#include "entity.h"
#include "packages/item.h"
#include "game.h"
#include "Path.h"
#include "sprite.h"
#include "tower/tower.h"

int main(int argc, char * argv[])
{
	OT::Application * a = new OT::Application(argc, argv);
	int exitCode = a->run();
	delete a;
	a = NULL;
	return exitCode;
	
	//Create the main window.
	sf::RenderWindow app(sf::VideoMode(800, 600, 32), "OpenSkyscraper SFML");
	
	//Limit the framerate.
	app.SetFramerateLimit(60);
	
	//Create a game instance.
	Game game;
	
	//Create a string to be drawn.
	sf::Font jura;
	jura.LoadFromFile("data/fonts/Jura-Regular.ttf", 16);
	
	sf::String text(L"Welcome to OpenSkyscraper!", jura, 16);
	text.SetColor(sf::Color(255, 255, 0));
	text.Move(0, 0);

	//Load an image to be drawn.
	sf::Image condo;
	condo.LoadFromFile("data/condo.png");
	condo.SetSmooth(false);
	
	//Create a entity that uses the image.
	Entity entity;
	entity.SetImage(condo);
	entity.SetPosition(80, 70);
	entity.z = 0;
	
	Entity entity2;
	entity2.SetImage(condo);
	entity2.SetPosition(130, 85);
	entity2.z = 1;
	
	//Create some space partition.
	game.addEntity(&entity);
	game.addEntity(&entity2);
	
	//Center the view.
	app.GetDefaultView().SetCenter(0, 0);
	
	//Expose the classes.
	Item::expose(game.lua);
	Sprite::expose(game.lua);
	
	//Create a new tower.
	Tower tower(&game);
	
	//Create a new item an try to simulate it.
	game.lua.dofile("data/debug/condo.lua");
	Item item(&tower, "CondoItem");
	item.simulate(0.13);
	
	//Run the main loop.
	bool visibleRectChanged = true;
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
				visibleRectChanged = true;
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
			
			//Entity movement.
			if (event.Type == sf::Event::KeyPressed) {
				switch (event.Key.Code) {
					case sf::Key::Left:		app.GetDefaultView().Move(-10, 0); break;
					case sf::Key::Right:	app.GetDefaultView().Move( 10, 0); break;
					case sf::Key::Up:		app.GetDefaultView().Move(0, -10); break;
					case sf::Key::Down:		app.GetDefaultView().Move(0,  10); break;
                    default: break;
				}
				visibleRectChanged = true;
			}
		}
		
		//Update the slice if required.
		if (visibleRectChanged) {
			sf::FloatRect rect = app.GetDefaultView().GetRect();
			rectd r(rect.Left + 200, rect.Top + 200,
					rect.Right - rect.Left - 400, rect.Bottom - rect.Top - 400);
			game.space.setVisibleRect(r);
			visibleRectChanged = false;
		}
		
		//Clear the screen.
		app.Clear();
		
		//Advance the game.
		double dt = std::min<double>(app.GetFrameTime(), 0.1);
		game.advance(dt);
		
		//Draw the text.
		app.Draw(text);
		
		//Draw the entities.
		//app.Draw(entity);
		const std::vector<Entity *> & entities = game.space.getSortedVisibleEntitys();
		for (std::vector<Entity *>::const_iterator sp = entities.begin(); sp != entities.end(); sp++)
			app.Draw(**sp);
		
		//Draw other stuff.
		game.space.draw(app);
		
		//Display whatever we have drawn so far
		app.Display();
	}
	return 0;
}
