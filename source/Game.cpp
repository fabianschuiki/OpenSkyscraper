#include <cassert>
#include "Application.h"
#include "Game.h"

using namespace OT;

Game::Game(Application & app)
:	State("game"),
	app(app),
	itemFactory(this),
	toolboxWindow(this),
	timeWindow(this),
	sky(this)
{
	mapWindow     = NULL;
	
	funds  = 4000000;
	rating = 0;
	
	time.set(5);
	paused = false;
	
	zoom = 1;
	poi.y = 200;
	
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
	
	//Advance time.
	if (paused) dt = 0;
	time.advance(dt);
	timeWindow.updateTime();
	
	timeWindow.advance(dt);
	sky.advance(dt);
	
	//Constrain the POI.
	double2 halfsize(win.GetWidth()*0.5*zoom, win.GetHeight()*0.5*zoom);
	poi.y = std::max<double>(std::min<double>(poi.y, 360*12 - halfsize.y), -360 + halfsize.y);
	
	//Adust the camera.
	sf::View cameraView(sf::Vector2f(poi.x, -poi.y), sf::Vector2f(halfsize.x, halfsize.y));
	win.SetView(cameraView);
	sf::FloatRect view = cameraView.GetRect();
	win.SetView(sf::View(view));
	
	win.Draw(sky);
	
	//Draw the items that are in view.
	for (ItemSet::iterator i = items.begin(); i != items.end(); i++) {
		const sf::Vector2f & vp = (*i)->GetPosition();
		const sf::Vector2f & vs = (*i)->GetSize();
		if (vp.x+vs.x >= view.Left && vp.x <= view.Right && vp.y >= view.Top && vp.y-vs.y <= view.Bottom) {
			win.Draw(**i);
		}
	}
	
	//Draw the debug string.
	snprintf(debugString, 512, "%i sprites", drawnSprites);
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
	xml.PushAttribute("time", time.absolute);
	xml.PushAttribute("paused", paused);
	xml.PushAttribute("rainy", sky.rainyDay);
	
	xml.PushAttribute("x", (int)poi.x);
	xml.PushAttribute("y", (int)poi.y);
	
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
	time.set(root->DoubleAttribute("time"));
	setPaused(root->BoolAttribute("paused"));
	sky.rainyDay = root->BoolAttribute("rainy");
	
	poi.x = root->IntAttribute("x");
	poi.y = root->IntAttribute("y");
	
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

void Game::setPaused(bool p)
{
	if (paused != p) {
		paused = p;
		toolboxWindow.updateSpeed();
	}
}
