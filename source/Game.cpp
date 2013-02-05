#include <cassert>
#include "Application.h"
#include "Game.h"
#include "Item/Lobby.h"

#ifdef _WIN32
#include "Math/Round.h"
#endif

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
	
	time.set(7/78.0);
	speedMode = 1;
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
	DataManager::Paths p = app.data.paths("default.tower");
	for (DataManager::Paths::iterator ip = p.begin(); ip != p.end(); ip++) {
		LOG(DEBUG, "trying %s", (*ip).c_str());
		if (xml.LoadFile(*ip) == 0) break;
	}
	decodeXML(xml);
}

Game::~Game()
{
	for (ItemSet::iterator i = items.begin(); i != items.end(); i++) delete *i;
	items.clear();
	itemsByFloor.clear();
	itemsByType.clear();
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
				case sf::Key::Left:  poi.x -= 20; return true;
				case sf::Key::Right: poi.x += 20; return true;
				case sf::Key::Up:    poi.y += 20; return true;
				case sf::Key::Down:  poi.y -= 20; return true;
				case sf::Key::F1:    reloadGUI(); return true;
				case sf::Key::F3:    setRating(1); return true;
				case sf::Key::F2: {
					FILE * f = fopen("default.tower", "w");
					tinyxml2::XMLPrinter xml(f);
					encodeXML(xml);
					fclose(f);
				} return true;
				case sf::Key::PageUp:   zoom /= 2; return true;
				case sf::Key::PageDown: zoom *= 2; return true;
			}
		} break;
		
		case sf::Event::TextEntered: {
			switch (event.Text.Unicode) {
				case '0': setSpeedMode(0); return true;
				case '1': setSpeedMode(1); return true;
				case '2': setSpeedMode(2); return true;
				case '3': setSpeedMode(3); return true;
			} break;
		} break;
		
		case sf::Event::MouseButtonPressed: {
			float2 mousePoint(event.MouseButton.X, event.MouseButton.Y);
			rectf toolboxWindowRect(float2(toolboxWindow.window->GetAbsoluteLeft(), toolboxWindow.window->GetAbsoluteTop()), float2(toolboxWindow.window->GetClientWidth(), toolboxWindow.window->GetClientHeight()));
			rectf timeWindowRect(float2(timeWindow.window->GetAbsoluteLeft(), timeWindow.window->GetAbsoluteTop()), float2(timeWindow.window->GetClientWidth(), timeWindow.window->GetClientHeight()));
			rectf mapWindowRect(float2(mapWindow->GetAbsoluteLeft(), mapWindow->GetAbsoluteTop()), float2(mapWindow->GetClientWidth(), mapWindow->GetClientHeight()));
			
			// Prevent construction or triggering of tool if mouse cursor within toolboxWindow
			if (toolboxWindowRect.containsPoint(mousePoint)) break;

			// Prevent construction or triggering of tool if mouse cursor within timeWindow
			if (timeWindowRect.containsPoint(mousePoint)) break;

			// Prevent construction or triggering of tool if mouse cursor within mapWindow
			if (mapWindowRect.containsPoint(mousePoint)) {
				break;	// Break for now, may add code to handle viewport shift in future
			}

			if (toolPrototype) {
				bool handled = false;
				recti toolBoundary = recti(toolPosition, toolPrototype->size);
				if (toolPrototype->id.find("elevator") == 0) {
					const ItemSet &elevators = itemsByType[toolPrototype->id];
					for (ItemSet::const_iterator i = elevators.begin(); i != elevators.end(); i++) {
						Item::Elevator::Elevator * e = (Item::Elevator::Elevator *) *i;
						recti itemRect = e->getRect();
						if (toolBoundary.minX() == itemRect.minX() && 
							toolBoundary.maxX() == itemRect.maxX() &&
							toolBoundary.minY() >= itemRect.minY() &&
							toolBoundary.maxY() <= itemRect.maxY()) {
							LOG(DEBUG, "add car on floor %i to elevator %s", toolPosition.y, e->desc().c_str());
							e->addCar(toolPosition.y);
							transferFunds(-80000);
							handled = true;
							break;
						}
					}
				}
				if (!handled) {
					bool constructionBlocked = false;
					int minFloorX = INT_MAX;
					int maxFloorX = INT_MIN;
					if (toolPrototype->id.compare("lobby") == 0) {
						if (toolPosition.y % 15 != 0) constructionBlocked = true;
						else if (toolPosition.y != 0) {
							// Check floor width below
							ItemSet itemsOnFloor = itemsByFloor[toolPosition.y - 1];
							for (ItemSet::const_iterator ii = itemsOnFloor.begin(); !constructionBlocked && ii != itemsOnFloor.end(); ii++) {
								Item::Item * i = *ii;
								if (i->canHaulPeople()) continue;
								minFloorX = std::min(minFloorX, i->position.x);
								maxFloorX = std::max(maxFloorX, i->getRect().maxX());
							}

							// Check for non-lobby items blocking construction on lobby floor
							itemsOnFloor = itemsByFloor[toolPosition.y];
							for (ItemSet::const_iterator ii = itemsOnFloor.begin(); !constructionBlocked && ii != itemsOnFloor.end(); ii++) {
								Item::Item * i = *ii;
								if (i->canHaulPeople() || i->prototype->id.compare("lobby") == 0) continue;
								minFloorX = std::max(minFloorX, i->getRect().maxX());
								maxFloorX = std::min(maxFloorX, i->position.x);
							}
						} else {
							minFloorX = INT_MIN;
							maxFloorX = INT_MAX;
						}
					} else if (toolPrototype->id.compare("stairs") == 0 || toolPrototype->id.compare("escalator") == 0) {
						// Check obstruction from other transport items
						const ItemSet &stairlike = itemsByType["stairlike"];
						for (ItemSet::const_iterator ii = stairlike.begin(); !constructionBlocked && ii != stairlike.end(); ii++) {
							Item::Item * i = *ii;
							if (i->position.y == toolPosition.y) {
								recti itemRect = i->getRect();
								if (toolBoundary.minX() >= itemRect.minX() && toolBoundary.minX() <= itemRect.maxX()) constructionBlocked = true;
							} else if (i->position.y == toolPosition.y - 1) {
								recti itemRect = i->getRect();
								if (toolBoundary.minX() > itemRect.minX() && toolBoundary.minX() < itemRect.maxX()) constructionBlocked = true;
							} else if (i->position.y == toolPosition.y + 1) {
								recti itemRect = i->getRect();
								if (toolBoundary.maxX() < itemRect.maxX() && toolBoundary.maxX() > itemRect.minX()) constructionBlocked = true;
							}
						}
						
						const ItemSet &elevators = itemsByType["elevator"];
						for (ItemSet::const_iterator ii = elevators.begin(); !constructionBlocked && ii != elevators.end(); ii++) {
							Item::Item * i = *ii;
							recti itemRect = i->getRect();
							itemRect.origin.y -= 1;
							itemRect.size.y += 2;
							if (toolBoundary.intersectsRect(itemRect)) constructionBlocked = true;
						}

						// Check floor width above
						ItemSet itemsNearby = itemsByFloor[toolPosition.y + 1];
						for (ItemSet::const_iterator ii = itemsNearby.begin(); !constructionBlocked && ii != itemsNearby.end(); ii++) {
							Item::Item * i = *ii;
							if (i->canHaulPeople()) continue;
							minFloorX = std::min(minFloorX, i->position.x);
							maxFloorX = std::max(maxFloorX, i->getRect().maxX());
						}
						if (itemsNearby.empty() || toolPosition.x < minFloorX || toolPosition.x + toolPrototype->size.x > maxFloorX)
							constructionBlocked = true;

						// Check floor width
						minFloorX = INT_MAX;
						maxFloorX = INT_MIN;
						itemsNearby = itemsByFloor[toolPosition.y];
						for (ItemSet::const_iterator ii = itemsNearby.begin(); !constructionBlocked && ii != itemsNearby.end(); ii++) {
							Item::Item * i = *ii;
							if (i->canHaulPeople()) continue;
							minFloorX = std::min(minFloorX, i->position.x);
							maxFloorX = std::max(maxFloorX, i->getRect().maxX());
						}
					} else if (toolPrototype->id.find("elevator") == 0) {
						// Check obstruction from other transport items
						toolBoundary.origin.y -= 1;
						toolBoundary.size.y += 2;

						const ItemSet &stairlike = itemsByType["stairlike"];
						for (ItemSet::const_iterator ii = stairlike.begin(); !constructionBlocked && ii != stairlike.end(); ii++) {
							Item::Item * i = *ii;
							if (toolBoundary.intersectsRect(i->getRect())) constructionBlocked = true;
						}

						const ItemSet &elevators = itemsByType["elevator"];
						for (ItemSet::const_iterator ii = elevators.begin(); !constructionBlocked && ii != elevators.end(); ii++) {
							Item::Item * i = *ii;
							recti itemRect = i->getRect();
							itemRect.origin.y -= 1;
							itemRect.size.y += 2;
							if (toolBoundary.intersectsRect(itemRect)) constructionBlocked = true;
						}

						// Check floor width below/above if constructing above/below ground level
						ItemSet itemsNearby;
						if (toolPosition.y > 0) itemsNearby = itemsByFloor[toolPosition.y - 1];
						else if (toolPosition.y < 0) itemsNearby = itemsByFloor[toolPosition.y + 1];
						else itemsNearby = itemsByFloor[0];
						for (ItemSet::const_iterator ii = itemsNearby.begin(); !constructionBlocked && ii != itemsNearby.end(); ii++) {
							Item::Item * i = *ii;
							if (i->canHaulPeople()) continue;
							minFloorX = std::min(minFloorX, i->position.x);
							maxFloorX = std::max(maxFloorX, i->getRect().maxX());
						}
					} else {
						if (toolPosition.y == 0) constructionBlocked = true;

						// Check obstruction from other buildings
						ItemSet itemsNearby;
						itemsNearby = itemsByFloor[toolPosition.y];
						for (ItemSet::const_iterator ii = itemsNearby.begin(); !constructionBlocked && ii != itemsNearby.end(); ii++) {
							Item::Item * i = *ii;
							if (i->canHaulPeople()) continue;
							if (toolBoundary.intersectsRect(i->getRect())) constructionBlocked = true;
						}

						// Check floor width below/above if constructing above/below ground level
						if (toolPosition.y > 0) itemsNearby = itemsByFloor[toolPosition.y - 1];
						else itemsNearby = itemsByFloor[toolPosition.y + 1];
						for (ItemSet::const_iterator ii = itemsNearby.begin(); !constructionBlocked && ii != itemsNearby.end(); ii++) {
							Item::Item * i = *ii;
							if (i->canHaulPeople()) continue;
							minFloorX = std::min(minFloorX, i->position.x);
							maxFloorX = std::max(maxFloorX, i->getRect().maxX());
						}
					}
					if (toolPosition.x < minFloorX || toolPosition.x + toolPrototype->size.x > maxFloorX)
						constructionBlocked = true;

					if (!constructionBlocked) {
						LOG(DEBUG, "construct %s at %ix%i, size %ix%i", toolPrototype->id.c_str(), toolPosition.x, toolPosition.y, toolPrototype->size.x, toolPrototype->size.y);
						if (toolPrototype->id.compare("lobby") == 0) {
							// Look for existing lobby to extend
							bool existingLobby = false;
							const ItemSet &itemsOnFloor = itemsByFloor[toolPosition.y];
							for (ItemSet::const_iterator ii = itemsOnFloor.begin(); !constructionBlocked && ii != itemsOnFloor.end(); ii++) {
								Item::Item * i = *ii;
								if (i->prototype->id.compare("lobby") == 0) {
									Item::Lobby * l = (Item::Lobby *) i;
									float diff = 0;
									if (toolPosition.x < l->position.x) {
										diff = l->position.x - toolPosition.x;
										l->size.x += diff;
										l->setPosition(toolPosition);
									} else {
										diff = toolPosition.x + toolPrototype->size.x - l->getRect().maxX();
										if (diff < 0) diff = 0;
										l->size.x += diff;
									}
									transferFunds(-toolPrototype->price * (diff/4));
									l->updateSprite();
									playOnce("simtower/construction/flexible");
									existingLobby = true;
								}
							}
							// Otherwise construct a new lobby
							if (!existingLobby) {
								Item::Item * item = itemFactory.make(toolPrototype, toolPosition);
								addItem(item);
								transferFunds(-toolPrototype->price);
								playOnce("simtower/construction/normal");
							}
						} else {
							Item::Item * item = itemFactory.make(toolPrototype, toolPosition);
							addItem(item);
							transferFunds(-toolPrototype->price);
							if (item->canHaulPeople()) {
								if (item->isElevator()) selectTool("finger");
								updateRoutes();
							} else
								item->updateRoutes();
							playOnce("simtower/construction/normal");
						}
					} else {
						LOG(DEBUG, "cannot construct %s at %ix%i, size %ix%i", toolPrototype->id.c_str(), toolPosition.x, toolPosition.y, toolPrototype->size.x, toolPrototype->size.y);
						playOnce("simtower/construction/impossible");
					}
				}
			}
			else if (itemBelowCursor) {
				if (selectedTool == "bulldozer") {
					if (itemBelowCursor->prototype->id.compare("lobby") == 0) {
						playOnce("simtower/construction/impossible");
						break;
					}
					LOG(DEBUG, "destroy %s", itemBelowCursor->desc().c_str());
					bool canHaulPeople = false;
					if (itemBelowCursor->canHaulPeople()) canHaulPeople = true;
					removeItem(itemBelowCursor);
					if (canHaulPeople) updateRoutes();
					playOnce("simtower/bulldozer");
				}
				else if (selectedTool == "finger") {
					if (itemBelowCursor->prototype->id.find("elevator") == 0) {
						Item::Elevator::Elevator * e = (Item::Elevator::Elevator *)itemBelowCursor;
						
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
							e->cleanQueues();
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
			if (draggingElevator && draggingElevator->repositionMotor(draggingMotor, toolPosition.y)) {
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
	if (time.checkHour(5))  cockSound.Play(this);
	if (time.checkHour(6))  morningSound.Play(this);
	if (time.checkHour(9))  bellsSound.Play(this);
	if (time.checkHour(18)) eveningSound.Play(this);
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
	
	//Adjust pitch of playing sounds.
	for (SoundSet::iterator s = playingSounds.begin(); s != playingSounds.end();) {
		if ((*s)->GetStatus() == sf::Sound::Stopped) {
			playingSounds.erase(s++);
		} else {
			(*s)->SetPitch(1 + (time.speed_animated-1) * 0.2);
			s++;
		}
	}
	
	//Autorelease sounds.
	for (SoundSet::iterator s = autoreleaseSounds.begin(); s != autoreleaseSounds.end();) {
		if ((*s)->GetStatus() == sf::Sound::Stopped) {
			delete *s;
			autoreleaseSounds.erase(s++);
		} else {
			s++;
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
	itemsByFloor[item->position.y].insert(item);
	itemsByType[item->prototype->id].insert(item);

	if (item->canHaulPeople()) {
		itemsByType["canHaulPeople"].insert(item);
		if (item->isElevator()) itemsByType["elevator"].insert(item);
		else itemsByType["stairlike"].insert(item);
	}
}

void Game::removeItem(Item::Item * item)
{
	assert(item);
	items.erase(item);
	itemsByFloor[item->position.y].erase(item);
	itemsByType[item->prototype->id].erase(item);
	if (item->canHaulPeople()) {
		itemsByType["canHaulPeople"].erase(item);
		if (item->isElevator()) itemsByType["elevator"].erase(item);
		else itemsByType["stairlike"].erase(item);
	}
	if (item == itemBelowCursor) itemBelowCursor = NULL;
}

void Game::encodeXML(tinyxml2::XMLPrinter & xml)
{
	xml.OpenElement("tower");
	xml.PushAttribute("funds", funds);
	xml.PushAttribute("rating", rating);
	xml.PushAttribute("time", time.absolute);
	xml.PushAttribute("speed", speedMode);
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
	setSpeedMode(root->IntAttribute("speed"));
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

void Game::transferFunds(int f, std::string message)
{
	setFunds(funds + f);
	playOnce("simtower/cash");
	if (!message.empty()) {
		char c[32];
		snprintf(c, 32, ": $%i", f);
		timeWindow.showMessage(message + c);
	}
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

void Game::setSpeedMode(int sm)
{
	assert(sm >= 0 && sm <= 3);
	if (speedMode != sm) {
		speedMode = sm;
		double speed = 0;
		switch (speedMode) {
			case 0: speed = 0; break;
			case 1: speed = 1; break;
			case 2: speed = 2; break;
			case 3: speed = 4; break;
		}
		time.speed = speed;
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
	//Make sure we don't play sounds to frequently.
	if (soundPlayTimes.count(sound)) {
		if (soundPlayTimes[sound] > time.absolute - 0.25) {
			return;
		}
	}
	soundPlayTimes[sound] = time.absolute;

	//Actually play the sound.
	Sound * snd = new Sound;
	snd->SetBuffer(app.sounds[sound]);
	snd->Play(this);
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

/** Finds a route from start to destination through the tower. The returned route contains start as
 *  the first and destination as the last node, with transportation in between. If the returned
 *  route is empty(), no path was found through the tower. */
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
		
		//Limit the number of elevators, stairs and escalators to use per route.
		if (i->isElevator() && route.numElevators >= 2) continue;
		if (i->prototype->id == "stairs"    && route.numStairs     >= 3) continue;
		if (i->prototype->id == "escalator" && route.numEscalators >= 5) continue;
		
		//Check whether this item connects to the floor we're currently at.
		if (!i->connectsFloor(floor)) continue;
		
		//If this item connects to our destination floor, use it.
		Route r;
		if (i->connectsFloor(destination->position.y)) {
			r = route;
			r.add(i, destination->position.y);
			r.add(destination);
		}
		
		//For elevators, check the lobbies for connecting transports.
		else if (i->isElevator()) {
			for (int n = i->position.y % 15; n < i->size.y; n += 15) {
				int f = n + i->position.y;
				if (!i->connectsFloor(f)) continue;
				Route sr = findRoute(route, f, i, destination);
				if (!sr.empty() && (r.empty() || sr.score() < r.score())) r = sr;
			}
		}
		
		//For stairlike items, check the floor above/below for connecting transports.
		else if (i->isStairlike()) {
			int f = i->position.y;
			if (f == floor) f += i->size.y-1;
			r = findRoute(route, f, i, destination);
		}
		
		//If r's score is lower (better) than the best we've found so far, make r the current best route.
		if (!r.empty() && (best.empty() || r.score() < best.score())) best = r;
	}
	
	return best;
}
