#include <cassert>
#include "Application.h"
#include "Game.h"

using namespace OT;

Game::Game(Application & app)
:	State("game"),
	app(app),
	itemFactory(this),
	toolboxWindow(this),
	timeWindow(this)
{
	mapWindow     = NULL;
	
	funds  = 4000000;
	rating = 0;
	
	zoom = 1;
	poi.y = 0;
	
	skyState = 0;
	
	itemFactory.loadPrototypes();
	
	/*Item::Item * i = itemFactory.prototypes.front()->make(this);
	addItem(i);
	i = itemFactory.prototypes.front()->make(this);
	i->setPosition(int2(20, 0));
	addItem(i);*/
	/*Sprite * s = new Sprite;
	s->SetImage(app.bitmaps["simtower/security"]);
	s->Resize(384, 24);
	s->SetCenter(0, 24);
	s->SetPosition(0, 0);
	sprites.insert(s);*/
	
	reloadGUI();
	
	//DEBUG: load from disk.
	tinyxml2::XMLDocument xml;
	xml.LoadFile("default.tower");
	decodeXML(xml);
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
				case sf::Key::F2: {
					FILE * f = fopen("default.tower", "w");
					tinyxml2::XMLPrinter xml(f);
					encodeXML(xml);
					fclose(f);
				} break;
				case sf::Key::PageUp:   zoom /= 2; break;
				case sf::Key::PageDown: zoom *= 2; break;
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
	sf::View cameraView(sf::Vector2f(poi.x, -poi.y), sf::Vector2f(win.GetWidth()*0.5*zoom, win.GetHeight()*0.5*zoom));
	win.SetView(cameraView);
	sf::FloatRect view = cameraView.GetRect();
	win.SetView(sf::View(view));
	
	//Draw the background.
	drawBackground(view);
	
	//Draw the sprites that are in view.
	/*for (Sprites::iterator s = sprites.begin(); s != sprites.end(); s++) {
		const sf::Vector2f & vp = (*s)->GetPosition();
		const sf::Vector2f & vs = (*s)->GetSize();
		if (vp.x+vs.x >= view.Left && vp.x <= view.Right && vp.y+vs.y >= view.Top && vp.y <= view.Bottom) {
			win.Draw(**s);
			drawnSprites++;
		}
	}*/
	
	//Draw the items that are in view.
	for (ItemSet::iterator i = items.begin(); i != items.end(); i++) {
		/*for (Item::Item::SpriteSet::iterator s = (*i)->sprites.begin(); s != (*i)->sprites.end(); s++) {
			const sf::Vector2f & vp = (*s)->GetPosition();
			const sf::Vector2f & vs = (*s)->GetSize();
			if (vp.x+vs.x >= view.Left && vp.x <= view.Right && vp.y+vs.y >= view.Top && vp.y <= view.Bottom) {
				win.Draw(**s);
				drawnSprites++;
			}
		}*/
		/*const sf::Vector2f & vp = (*i)->GetPosition();
		const sf::Vector2f & vs = (*i)->GetSize();
		if (vp.x+vs.x >= view.Left && vp.x <= view.Right && vp.y+vs.y >= view.Top && vp.y <= view.Bottom) {*/
			win.Draw(**i);
			//drawnSprites++;
		//}
	}
	
	//Draw the debug string.
	snprintf(debugString, 512, "%i sprites", drawnSprites);
}

void Game::drawBackground(const sf::FloatRect & rect)
{
	//Draw the sky color.
	int sky_lower = std::max<int>(floor(-rect.Bottom / 360), -1);
	int sky_upper = std::min<int>(ceil (-rect.Top    / 360), 11);
	
	Sprite sky;
	sky.SetImage(app.bitmaps["simtower/sky"]);
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
		Sprite city;
		city.SetImage(app.bitmaps["simtower/deco/skyline"]);
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
	if (mapWindow) {
		mapWindow->RemoveReference();
		mapWindow->Close();
	}
	
	mapWindow     = gui.loadDocument("map.rml");
	
	if (mapWindow)     mapWindow    ->Show();
	
	toolboxWindow.reload();
	timeWindow.reload();
}

void Game::addItem(Item::Item * item)
{
	assert(item);
	items.insert(item);
}

void Game::removeItem(Item::Item * item)
{
	assert(item);
	items.erase(item);
}

void Game::encodeXML(tinyxml2::XMLPrinter & xml)
{
	xml.OpenElement("tower");
	xml.PushAttribute("funds", funds);
	xml.PushAttribute("rating", rating);
	
	for (ItemSet::iterator i = items.begin(); i != items.end(); i++) {
		xml.OpenElement("item");
		(*i)->encodeXML(xml);
		xml.CloseElement();
	}
	
	xml.CloseElement();
}

void Game::decodeXML(tinyxml2::XMLDocument & xml)
{
	tinyxml2::XMLElement * root = xml.RootElement();
	assert(root);
	
	setFunds(root->IntAttribute("funds"));
	setRating(root->IntAttribute("rating"));
	
	tinyxml2::XMLElement * e = root->FirstChildElement("item");
	while (e) {
		Item::Item * item = itemFactory.make(*e);
		addItem(item);
		e = e->NextSiblingElement("item");
	}
}

void Game::transferFunds(int f)
{
	setFunds(funds + f);
	LOG(DEBUG, "%i", f);
}

void Game::setFunds(int f)
{
	if (funds != f) {
		funds = f;
		timeWindow.updateFunds();
	}
}

void Game::setRating(int r)
{
	if (rating != r) {
		bool improved = (r > rating);
		rating = r;
		if (improved) {
			//TODO: show window
			LOG(IMPORTANT, "rating increased to %i", rating);
		}
		timeWindow.updateRating();
	}
}
