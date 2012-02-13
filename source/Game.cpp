#include "Application.h"
#include "Game.h"

using namespace OT;

Game::Game(Application & app)
:	app(app)
{
	zoom = 1;
	
	Sprite * s = new Sprite;
	s->SetImage(app.simtower.bitmaps["security"]);
	s->Resize(384, 24);
	s->SetCenter(0, 0);
	sprites.insert(s);
}

void Game::activate()
{
	
}

void Game::deactivate()
{
	
}

bool Game::handleEvent(sf::Event & event)
{
	return false;
}

void Game::advance(double dt)
{
	sf::RenderWindow & win = app.window;
	
	//Adust the camera.
	sf::View cameraView(poi, sf::Vector2f(win.GetWidth()*0.5*zoom, win.GetHeight()*0.5*zoom));
	win.SetView(cameraView);
	sf::FloatRect view = cameraView.GetRect();
	
	//Draw the sprites that are in view.
	for (Sprites::iterator s = sprites.begin(); s != sprites.end(); s++) {
		const sf::Vector2f & vp = (*s)->GetPosition();
		const sf::Vector2f & vs = (*s)->GetSize();
		if (vp.x+vs.x >= view.Left && vp.x <= view.Right && vp.y+vs.y >= view.Top && vp.y <= view.Bottom) {
			win.Draw(**s);
		}
	}
}
