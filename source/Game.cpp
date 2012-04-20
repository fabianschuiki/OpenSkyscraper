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
	population = 0;
	populationNeedsUpdate = false;
	
	time.set(5);
	paused = false;
	selectedTool = "inspector";
	itemBelowCursor = NULL;
	toolPrototype = NULL;
	
	zoom = 1;
	poi.y = 200;
	
	draggingElevator = NULL;
	draggingMotor = 0;
	
	mainLobby = NULL;
	
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
	
	cockSound.SetBuffer(app.sounds["simtower/cock"]);
	morningSound.SetBuffer(app.sounds["simtower/birds/morning"]);
	bellsSound.SetBuffer(app.sounds["simtower/bells"]);
	eveningSound.SetBuffer(app.sounds["simtower/birds/evening"]);
	
	//DEBUG: load from disk.
	tinyxml2::XMLDocument xml;
	xml.LoadFile("default.tower");
	decodeXML(xml);
}

Game::~Game()
{
	for (ItemSet::iterator i = items.begin(); i != items.end(); i++) delete *i;
	items.clear();
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
				case sf::Key::F3:    setRating(1); break;
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
		
		case sf::Event::MouseButtonPressed: {
			if (toolPrototype) {
				bool handled = false;
				if (toolPrototype->id.find("elevator") == 0) {
					for (ItemSet::iterator i = items.begin(); i != items.end(); i++) {
						if ((*i)->prototype == toolPrototype && (*i)->getRect().containsPoint(toolPosition)) {
							LOG(DEBUG, "add car on floor %i to elevator %s", toolPosition.y, (*i)->desc().c_str());
							((Item::Elevator *)*i)->addCar(toolPosition.y);
							transferFunds(-80000);
							handled = true;
							break;
						}
					}
				}
				if (!handled) {
					LOG(DEBUG, "construct %s at %ix%i", toolPrototype->id.c_str(), toolPosition.x, toolPosition.y);
					Item::Item * item = itemFactory.make(toolPrototype, toolPosition);
					addItem(item);
					transferFunds(-toolPrototype->price);
					updateRoutes();
					playOnce("simtower/construction/normal");
				}
			}
			else if (itemBelowCursor) {
				if (selectedTool == "bulldozer") {
					LOG(DEBUG, "destroy %s", itemBelowCursor->desc().c_str());
					removeItem(itemBelowCursor);
					updateRoutes();
					playOnce("simtower/bulldozer");
				}
				else if (selectedTool == "finger") {
					if (itemBelowCursor->prototype->id.find("elevator") == 0) {
						Item::Elevator * e = (Item::Elevator *)itemBelowCursor;
						
						draggingMotor = 0;
						if (toolPosition.y < itemBelowCursor->position.y) draggingMotor = -1;
						if (toolPosition.y >= itemBelowCursor->position.y + itemBelowCursor->size.y) draggingMotor = 1;
						
						if (draggingMotor != 0) {
							LOG(DEBUG, "drag elevator %s motor %i", itemBelowCursor->desc().c_str(), draggingMotor);
							draggingElevator = e;
						} else {
							LOG(DEBUG, "clicked elevator %s on floor %i", itemBelowCursor->desc().c_str(), toolPosition.y);
							if (!e->unservicedFloors.erase(toolPosition.y))
								e->unservicedFloors.insert(toolPosition.y);
							updateRoutes();
						}
					}
				}
				else if (selectedTool == "inspector") {
					LOG(DEBUG, "inspect %s", itemBelowCursor->desc().c_str());
					visualizeRoute = itemBelowCursor->lobbyRoute;
					char c[128];
					snprintf(c, 128, "route score = %i", visualizeRoute.score());
					timeWindow.showMessage(c);
				}
			}
		} break;
		
		case sf::Event::MouseMoved: {
			if (draggingElevator) {
				draggingElevator->repositionMotor(draggingMotor, toolPosition.y);
				updateRoutes();
			}
		} break;
		
		case sf::Event::MouseButtonReleased: {
			draggingElevator = NULL;
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
	
	for (ItemSet::iterator i = items.begin(); i != items.end(); i++) {
		(*i)->advance(dt);
	}
	
	if (populationNeedsUpdate) {
		populationNeedsUpdate = false;
		int p = 0;
		for (ItemSet::iterator i = items.begin(); i != items.end(); i++) {
			p += (*i)->population;
		}
		setPopulation(p);
	}
	
	//Play sounds.
	if (time.checkHour(5)) cockSound.Play();
	if (time.checkHour(6))   morningSound.Play();
	if (time.checkHour(9))   bellsSound.Play();
	if (time.checkHour(18))  eveningSound.Play();
	morningSound.SetLoop(time.hour < 8);
	
	//Constrain the POI.
	double2 halfsize(win.GetWidth()*0.5*zoom, win.GetHeight()*0.5*zoom);
	poi.y = std::max<double>(std::min<double>(poi.y, 360*12 - halfsize.y), -360 + halfsize.y);
	
	//Adust the camera.
	sf::View cameraView(sf::Vector2f(poi.x, -poi.y), sf::Vector2f(halfsize.x, halfsize.y));
	win.SetView(cameraView);
	sf::FloatRect view = cameraView.GetRect();
	//win.SetView(sf::View(view));
	
	//Prepare the current tool.
	const sf::Input & input = win.GetInput();
	sf::Vector2f mp = win.ConvertCoords(input.GetMouseX(), input.GetMouseY());
	Item::AbstractPrototype * previousPrototype = toolPrototype;
	if (selectedTool.find("item-") == 0) {
		toolPrototype = itemFactory.prototypesById[selectedTool.substr(5)];
		toolPosition = int2(round(mp.x/8-toolPrototype->size.x/2.0), round(-mp.y/36-toolPrototype->size.y/2.0));
	} else {
		toolPrototype = NULL;
		toolPosition = int2(floor(mp.x/8), floor(-mp.y/36));
	}
	if (previousPrototype != toolPrototype) timeWindow.updateTooltip();
	
	//Draw the sky.
	win.Draw(sky);
	
	//Draw the items that are in view.
	Item::Item * previousItemBelowCursor = itemBelowCursor;
	itemBelowCursor = NULL;
	for (int layer = 0; layer < 2; layer++) {
		for (ItemSet::iterator i = items.begin(); i != items.end(); i++) {
			if ((*i)->layer != layer) continue;
			const sf::Vector2f & vp = (*i)->GetPosition();
			const sf::Vector2f & vs = (*i)->GetSize();
			if (vp.x+vs.x >= view.Left && vp.x <= view.Right && vp.y >= view.Top && vp.y-vs.y <= view.Bottom) {
				win.Draw(**i);
				if ((*i)->getMouseRegion().containsPoint(double2(mp.x, mp.y))) itemBelowCursor = *i;
			}
		}
	}
	
	//Highlight the item below the cursor.
	if (!toolPrototype && itemBelowCursor) {
		sf::Sprite s;
		s.Resize(itemBelowCursor->GetSize().x, itemBelowCursor->GetSize().y-12);
		s.SetCenter(0, 1);
		s.SetPosition(itemBelowCursor->GetPosition());
		s.SetColor(sf::Color(255, 255, 255, 255*0.5));
		win.Draw(s);
		drawnSprites++;
		if (previousItemBelowCursor != itemBelowCursor) {
			timeWindow.showMessage(itemBelowCursor->prototype->name);
		}
	}
	
	//Draw construction template.
	glBindTexture(GL_TEXTURE_2D, 0);
	if (toolPrototype) {
		rectf r(toolPosition.x * 8, -(toolPosition.y+toolPrototype->size.y) * 36, toolPrototype->size.x*8, toolPrototype->size.y*36);
		r.inset(float2(0.5, 0.5));
		glColor3f(1, 1, 1);
		glBegin(GL_LINE_STRIP);
		glVertex2f(r.minX(), r.minY());
		glVertex2f(r.maxX(), r.minY());
		glVertex2f(r.maxX(), r.maxY());
		glVertex2f(r.minX(), r.maxY());
		glVertex2f(r.minX(), r.minY());
		glEnd();
	}
	
	//Visualize route.
	glColor3f(0, 1, 0);
	glBegin(GL_LINE_STRIP);
	int prevFloor = 0;
	if (!visualizeRoute.nodes.empty()) prevFloor = visualizeRoute.nodes[0].item->position.y;
	for (std::vector<Route::Node>::iterator nit = visualizeRoute.nodes.begin(); nit != visualizeRoute.nodes.end(); nit++) {
		Route::Node & n = *nit;
		rectf r = n.item->getRect();
		if (n.item != mainLobby) {
			glVertex2f(r.midX()*8, -prevFloor * 36 - 5);
			glVertex2f(r.midX()*8, -n.toFloor * 36 - 5);
		}
		prevFloor = n.toFloor;
	}
	glEnd();
	
	//Autorelease sounds.
	for (SoundSet::iterator s = autoreleaseSounds.begin(); s != autoreleaseSounds.end(); s++) {
		if ((*s)->GetStatus() == sf::Sound::Stopped) {
			delete *s;
			autoreleaseSounds.erase(s);
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
	if (item == itemBelowCursor) itemBelowCursor = NULL;
}

void Game::encodeXML(tinyxml2::XMLPrinter & xml)
{
	xml.OpenElement("tower");
	xml.PushAttribute("funds", funds);
	xml.PushAttribute("rating", rating);
	xml.PushAttribute("time", time.absolute);
	xml.PushAttribute("paused", paused);
	xml.PushAttribute("rainy", sky.rainyDay);
	xml.PushAttribute("tool", selectedTool.c_str());
	
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
	selectTool(root->Attribute("tool"));
	
	poi.x = root->IntAttribute("x");
	poi.y = root->IntAttribute("y");
	
	tinyxml2::XMLElement * e = root->FirstChildElement("item");
	while (e) {
		Item::Item * item = itemFactory.make(*e);
		addItem(item);
		e = e->NextSiblingElement("item");
	}
	updateRoutes();
}

void Game::transferFunds(int f)
{
	setFunds(funds + f);
	playOnce("simtower/cash");
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
			playOnce("simtower/rating/increased");
		}
		timeWindow.updateRating();
	}
}

void Game::setPopulation(int p)
{
	if (population != p) {
		population = p;
		ratingMayIncrease();
		timeWindow.updatePopulation();
	}
}

/** Called whenever an event occurs that might allow the rating to increase, e.g. a change in
 *  population, or an item constructed. */
void Game::ratingMayIncrease()
{
	switch (rating) {
		case 0: {
			if (population >= 300) setRating(1);
		} break;
		case 1: {
			if (population >= 1000) {
				//TODO: check for security center presence.
				timeWindow.showMessage("Your tower needs security.");
			}
		} break;
	}
}

void Game::setPaused(bool p)
{
	if (paused != p) {
		paused = p;
		toolboxWindow.updateSpeed();
	}
}

void Game::selectTool(const char * tool)
{
	if (!tool) return;
	if (selectedTool != tool) {
		selectedTool = tool;
		toolboxWindow.updateTool();
		timeWindow.updateTooltip();
	}
}

/** Starts playing the given sound resource once. Releasing the sf::Sound after playback is done
 *  internally. */
void Game::playOnce(Path sound)
{
	sf::Sound * snd = new sf::Sound;
	snd->SetBuffer(app.sounds[sound]);
	snd->Play();
	autoreleaseSounds.insert(snd);
}

/** Called whenever the transportation layout of the tower changes. Items and people should update
 *  their calculated routes here, so they may find new ways of accessing things. */
void Game::updateRoutes()
{
	LOG(DEBUG, "updating routes");
	visualizeRoute.clear();
	for (ItemSet::iterator i = items.begin(); i != items.end(); i++) {
		(*i)->updateRoutes();
	}
}

Route Game::findRoute(Item::Item * start, Item::Item * destination)
{
	return findRoute(Route(), start->position.y, start, destination);
}

Route Game::findRoute(Route route, int floor, Item::Item * current, Item::Item * destination)
{
	route.add(current, floor);
	if (floor == destination->position.y) {
		route.add(destination);
		return route;
	}
	
	Route best;
	for (ItemSet::iterator it = items.begin(); it != items.end(); it++) {
		Item::Item * i = *it;
		if (!i->canHaulPeople() || route.usesItem(i)) continue;
		
		//Check whether this item connects to the floor we're currently at.
		if (!i->connectsFloor(floor)) continue;
		
		//If this item connects to our destination floor, use it.
		Route r;
		if (i->connectsFloor(destination->position.y)) {
			r = route;
			r.add(i, destination->position.y);
			r.add(destination);
		}
		
		//Otherwise check lobbies for elevators.
		else if (i->isElevator()) {
			for (int f = i->position.y % 15; f < i->size.y; f += 15) {
				Route sr = findRoute(route, f + i->position.y, i, destination);
				if (r.empty() || sr.score() < r.score()) r = sr;
			}
		}
		
		//Or the floor above/below for stairs.
		else if (i->isStairlike()) {
			int f = i->position.y;
			if (f == floor) f += i->size.y-1;
			r = findRoute(route, f, i, destination);
		}
		
		if (!r.empty() && (best.empty() || r.score() < best.score())) best = r;
	}
	
	return best;
}
