#include "Application.h"
#include "Game.h"

using namespace OT;

Game::Game(Application & app)
:	State(),
	app(app)
{
	timeWindow    = NULL;
	toolboxWindow = NULL;
	mapWindow     = NULL;
	
	zoom = 1;
	poi.y = 200;
	
	skyState = 0;
	
	Sprite * s = new Sprite;
	s->SetImage(app.bitmaps["security"]);
	s->Resize(384, 24);
	s->SetCenter(0, 24);
	s->SetPosition(0, 0);
	sprites.insert(s);
	
	reloadGUI();
}

void Game::activate()
{
	State::activate();
}

void Game::deactivate()
{
	State::deactivate();
}

bool Game::handleEvent(sf::Event & event)
{
	switch (event.Type) {
		case sf::Event::KeyPressed: {
			switch (event.Key.Code) {
				case sf::Key::Left:  poi.x -= 20; break;
				case sf::Key::Right: poi.x += 20; break;
				case sf::Key::Up:    poi.y += 20; break;
				case sf::Key::Down:  poi.y -= 20; break;
				case sf::Key::F1:    reloadGUI(); break;
			}
		} break;
	}
	return false;
}

void Game::advance(double dt)
{
	sf::RenderWindow & win = app.window;
	drawnSprites = 0;
	
	//Adust the camera.
	sf::Vector2f poi_inv = poi;
	poi_inv.y = -poi_inv.y;
	sf::View cameraView(poi_inv, sf::Vector2f(win.GetWidth()*0.5*zoom, win.GetHeight()*0.5*zoom));
	win.SetView(cameraView);
	sf::FloatRect view = cameraView.GetRect();
	
	//Draw the background.
	drawBackground(view);
	
	//Draw the sprites that are in view.
	for (Sprites::iterator s = sprites.begin(); s != sprites.end(); s++) {
		const sf::Vector2f & vp = (*s)->GetPosition();
		const sf::Vector2f & vs = (*s)->GetSize();
		if (vp.x+vs.x >= view.Left && vp.x <= view.Right && vp.y+vs.y >= view.Top && vp.y <= view.Bottom) {
			win.Draw(**s);
			drawnSprites++;
		}
	}
	
	//Draw the debug string.
	snprintf(debugString, 512, "%i sprites", drawnSprites);
}

void Game::drawBackground(const sf::FloatRect & rect)
{
	//Draw the sky color.
	int sky_lower = std::max<int>(floor(-rect.Bottom / 360), -1);
	int sky_upper = std::min<int>(ceil (-rect.Top    / 360), 11);
	
	sf::Sprite sky;
	sky.SetImage(app.bitmaps["sky"]);
	for (int y = sky_lower; y <= sky_upper; y++) {
		int index = (std::min<int>(y + 1, 9) * 6 + skyState);
		sky.SetSubRect(sf::IntRect(index * 32, 0, index * 32 + 32, 360));
		sky.Resize(32, 360);
		sky.SetCenter(0, 360);
		
		for (int x = floor(rect.Left / 32); x < ceil(rect.Right / 32); x++) {
			sky.SetPosition(x * 32, -y * 360);
			app.window.Draw(sky);
			drawnSprites++;
		}
	}
	
	//Draw the skyline, if in view.
	if (-rect.Bottom <= 96 && -rect.Top >= 0) {
		sf::Sprite city;
		city.SetImage(app.bitmaps["deco/skyline"]);
		city.SetCenter(0, 55);
		for (int x = floor(rect.Left / 96); x < ceil(rect.Right / 96); x++) {
			city.SetPosition(x * 96, 0);
			app.window.Draw(city);
			drawnSprites++;
		}
	}
}

void Game::reloadGUI()
{
	if (timeWindow) {
		timeWindow->RemoveReference();
		timeWindow->Close();
	}
	if (toolboxWindow) {
		toolboxWindow->RemoveReference();
		toolboxWindow->Close();
	}
	if (mapWindow) {
		mapWindow->RemoveReference();
		mapWindow->Close();
	}
	
	timeWindow    = gui.loadDocument("time.rml");
	toolboxWindow = gui.loadDocument("toolbox.rml");
	mapWindow     = gui.loadDocument("map.rml");
	
	if (timeWindow)    timeWindow   ->Show();
	if (toolboxWindow) toolboxWindow->Show();
	if (mapWindow)     mapWindow    ->Show();
}
