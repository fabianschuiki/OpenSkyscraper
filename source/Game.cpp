#include <cassert>
#include "Application.h"
#include "Game.h"
#include "Item/Lobby.h"
#include "GL/gl.h"

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
	sky(this),
	decorations(this)
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
	metroStation = NULL;
	
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
	
	cockSound.setBuffer(app.sounds["simtower/cock"]);
	morningSound.setBuffer(app.sounds["simtower/birds/morning"]);
	bellsSound.setBuffer(app.sounds["simtower/bells"]);
	eveningSound.setBuffer(app.sounds["simtower/birds/evening"]);
	
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
	switch (event.type) {
		case sf::Event::KeyPressed: {
			switch (event.key.code) {
				case sf::Keyboard::Left:  poi.x -= 20; return true;
				case sf::Keyboard::Right: poi.x += 20; return true;
				case sf::Keyboard::Up:    poi.y += 20; return true;
				case sf::Keyboard::Down:  poi.y -= 20; return true;
				case sf::Keyboard::F1:    reloadGUI(); return true;
				case sf::Keyboard::F3:    setRating(1); return true;
				case sf::Keyboard::F2: {
					FILE * f = fopen("default.tower", "w");
					tinyxml2::XMLPrinter xml(f);
					encodeXML(xml);
					fclose(f);
				} return true;
				case sf::Keyboard::PageUp:   zoom /= 2; return true;
				case sf::Keyboard::PageDown: zoom *= 2; return true;
			}
		} break;
		
		case sf::Event::TextEntered: {
			switch (event.text.unicode) {
				case '0': setSpeedMode(0); return true;
				case '1': setSpeedMode(1); return true;
				case '2': setSpeedMode(2); return true;
				case '3': setSpeedMode(3); return true;
			} break;
		} break;
		
		case sf::Event::MouseButtonPressed: {
			float2 mousePoint(event.mouseButton.x, event.mouseButton.y);
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
					std::string blockReason;
					int minFloorX = INT_MAX;
					int maxFloorX = INT_MIN;

					if (toolPosition.y < -9 && toolPrototype->icon != 19) {
						constructionBlocked = true;
						blockReason = "Cannot build below floor B9";
					}

					if (toolPosition.y > 0 && toolPrototype->icon == 19) {
						constructionBlocked = true;
						blockReason = toolPrototype->name + " unavailable above ground";
					}

					if (toolPrototype->icon == 0) {
						if (toolPosition.y % 15 != 0) {
							constructionBlocked = true;
							blockReason = "Lobbies can only be built on every 15th floor";
						}
						else if (toolPosition.y != 0) {
							// Check floor width below
							if (floorItems.count(toolPosition.y - 1) != 0) {
								Item::Item * i = floorItems[toolPosition.y - 1];
								minFloorX = i->position.x;
								maxFloorX = i->getRect().maxX();
							}

							// Check for non-lobby items blocking construction on lobby floor
							ItemSet itemsOnFloor = itemsByFloor[toolPosition.y];
							for (ItemSet::const_iterator ii = itemsOnFloor.begin(); !constructionBlocked && ii != itemsOnFloor.end(); ii++) {
								Item::Item * i = *ii;
								if (i->canHaulPeople() || i->prototype->icon == 0) continue;
								minFloorX = std::max(minFloorX, i->getRect().maxX());
								maxFloorX = std::min(maxFloorX, i->position.x);
							}
						} else {
							minFloorX = INT_MIN;
							maxFloorX = INT_MAX;
						}
					} else if (toolPrototype->icon == 2 || toolPrototype->icon == 3) {
						// Check obstruction from other transport items
						const ItemSet &stairlike = itemsByType["stairlike"];
						for (ItemSet::const_iterator ii = stairlike.begin(); !constructionBlocked && ii != stairlike.end(); ii++) {
							Item::Item * i = *ii;
							int xOffset = (toolPosition.x - i->position.x);
							if ((i->position.y == toolPosition.y && xOffset > -4 && xOffset < 4) ||
								(i->position.y == toolPosition.y - 1 && xOffset > 0 && xOffset < 8) ||
								(i->position.y == toolPosition.y + 1 && xOffset > -8 && xOffset < 0)) {
								constructionBlocked = true;
								blockReason = "Other " + i->prototype->name + " is in the way";
							}
						}
						
						const ItemSet &elevators = itemsByType["elevator"];
						for (ItemSet::const_iterator ii = elevators.begin(); !constructionBlocked && ii != elevators.end(); ii++) {
							Item::Item * i = *ii;
							recti itemRect = i->getRect();
							itemRect.origin.y -= 1;
							itemRect.size.y += 2;
							if (toolBoundary.intersectsRect(itemRect)) {
								constructionBlocked = true;
								blockReason = i->prototype->name + " is in the way";
							}
						}

						// Check floor width above
						if (floorItems.count(toolPosition.y + 1) != 0) {
							Item::Item * i = floorItems[toolPosition.y + 1];
							minFloorX = i->position.x;
							maxFloorX = i->getRect().maxX();
						}
						if (toolPosition.x < minFloorX || toolPosition.x + toolPrototype->size.x > maxFloorX) {
							if (!constructionBlocked)
								blockReason = "Upper floor is not wide enough";
							constructionBlocked = true;
						}

						// Check floor width
						minFloorX = INT_MAX;
						maxFloorX = INT_MIN;
						if (floorItems.count(toolPosition.y) != 0) {
							Item::Item * i = floorItems[toolPosition.y];
							minFloorX = i->position.x;
							maxFloorX = i->getRect().maxX();
						}
					} else if (toolPrototype->id.find("elevator") == 0) {
						// Check obstruction from other transport items
						toolBoundary.origin.y -= 1;
						toolBoundary.size.y += 2;

						const ItemSet &stairlike = itemsByType["stairlike"];
						for (ItemSet::const_iterator ii = stairlike.begin(); !constructionBlocked && ii != stairlike.end(); ii++) {
							Item::Item * i = *ii;
							if (toolBoundary.intersectsRect(i->getRect())) {
								constructionBlocked = true;
								blockReason = i->prototype->name + " is in the way";
							}
						}

						const ItemSet &elevators = itemsByType["elevator"];
						for (ItemSet::const_iterator ii = elevators.begin(); !constructionBlocked && ii != elevators.end(); ii++) {
							Item::Item * i = *ii;
							recti itemRect = i->getRect();
							itemRect.origin.y -= 1;
							itemRect.size.y += 2;
							if (toolBoundary.intersectsRect(itemRect)) {
								constructionBlocked = true;
								blockReason = "Other " + i->prototype->name + " is in the way";
							}
						}

						// Check floor width below/above if constructing above/below ground level
						Item::Item * i = NULL;
						if (toolPosition.y > 0 && floorItems.count(toolPosition.y - 1) != 0)
							i = floorItems[toolPosition.y - 1];
						else if (toolPosition.y < 0 && floorItems.count(toolPosition.y + 1) != 0)
							i = floorItems[toolPosition.y + 1];
						else if (floorItems.count(0) != 0)
							i = floorItems[0];
						if (i) {
							minFloorX = i->position.x;
							maxFloorX = i->getRect().maxX();
						}
					} else {
						if (toolPrototype->icon == 19 && metroStation != NULL) {
							constructionBlocked = true;
							blockReason = "Only one Metro Station allowed";
						}
						
						if (toolPosition.y == 0) {
							constructionBlocked = true;
							blockReason = "Only lobbies may be built on the ground floor";
						}

						// Check obstruction from other buildings
						ItemSet itemsNearby;
						for (int y = 0; !constructionBlocked && y < toolPrototype->size.y; y++) {
							itemsNearby = itemsByFloor[toolPosition.y + y];
							for (ItemSet::const_iterator ii = itemsNearby.begin(); !constructionBlocked && ii != itemsNearby.end(); ii++) {
								Item::Item * i = *ii;
								if (i->canHaulPeople()) continue;
								if (toolBoundary.intersectsRect(i->getRect())) {
									constructionBlocked = true;
									blockReason = i->prototype->name + " is in the way";
								}
							}
						}

						// Check floor width below/above if constructing above/below ground level
						Item::Item * i = NULL;
						if (toolPosition.y > 0 && floorItems.count(toolPosition.y - 1) != 0)
							i = floorItems[toolPosition.y - 1];
						else if (floorItems.count(toolPosition.y + toolPrototype->size.y) != 0)
							i = floorItems[toolPosition.y + toolPrototype->size.y];
						if (i) {
							minFloorX = i->position.x;
							maxFloorX = i->getRect().maxX();
						}
					}
					if (toolPosition.x < minFloorX || toolPosition.x + toolPrototype->size.x > maxFloorX) {
						if (!constructionBlocked)
							blockReason = "Floor " + std::string(toolPosition.y > 0 ? "below" : "above") + " is not wide enough";
						constructionBlocked = true;
					}

					if (!constructionBlocked) {
						LOG(DEBUG, "construct %s at %ix%i, size %ix%i", toolPrototype->id.c_str(), toolPosition.x, toolPosition.y, toolPrototype->size.x, toolPrototype->size.y);

						// Construct floors
						for (int i = 0; i < toolPrototype->size.y; i++)
							extendFloor(toolPosition.y + i, toolPosition.x, toolPosition.x + toolPrototype->size.x);

						if (toolPrototype->icon == 0) {
							// Look for existing lobby to extend
							bool existingLobby = false;
							const ItemSet &itemsOnFloor = itemsByFloor[toolPosition.y];
							for (ItemSet::const_iterator ii = itemsOnFloor.begin(); !existingLobby && ii != itemsOnFloor.end(); ii++) {
								Item::Item * i = *ii;
								if (i->prototype->icon == 0) {
									gameMap.removeNode(MapNode::Point(i->position.x + i->size.x/2, i->position.y), i);
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
									l->updateSprite();
									gameMap.addNode(MapNode::Point(i->position.x + i->size.x/2, i->position.y), i);
									if (diff > 0) {
										transferFunds(-toolPrototype->price * (diff/4));
										playOnce("simtower/construction/flexible");
									}
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
						} else if (toolPrototype->icon != 1) {
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
						timeWindow.showMessage("Cannot place item there. " + blockReason + ".");
					}
				}
			}
			else if (itemBelowCursor) {
				if (selectedTool == "bulldozer") {
					if (itemBelowCursor->prototype->icon == 0 || itemBelowCursor->prototype->icon == 1 || itemBelowCursor->prototype->icon == 19) {
						playOnce("simtower/construction/impossible");
						timeWindow.showMessage("Cannot bulldoze " + itemBelowCursor->prototype->name);
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
							draggingElevatorStart = toolPosition.y;
							if (draggingElevatorStart < draggingElevator->position.y) {
								draggingElevatorLower = true;
								draggingElevatorStart++;
							} else {
								draggingElevatorLower = false;
								draggingElevatorStart--;
							}
						} else {
							LOG(DEBUG, "clicked elevator %s on floor %i", itemBelowCursor->desc().c_str(), toolPosition.y);
							if (!e->unservicedFloors.erase(toolPosition.y)) {
								e->unservicedFloors.insert(toolPosition.y);
								gameMap.removeNode(MapNode::Point(e->position.x + e->size.x/2, toolPosition.y), itemBelowCursor);
							} else {
								gameMap.addNode(MapNode::Point(e->position.x + e->size.x/2, toolPosition.y), itemBelowCursor);
							}
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
				// Construct floors
				if (draggingElevatorLower) {
					if (draggingElevatorStart > draggingElevator->position.y) {
						for(int i = draggingElevator->position.y; i < draggingElevatorStart ; i++) {
							extendFloor(i, draggingElevator->position.x, draggingElevator->getRect().maxX());
						}
					}
				} else {
					recti rect = draggingElevator->getRect();
					if (draggingElevatorStart < rect.maxY() - 1) {
						for(int i = rect.maxY() - 1; i > draggingElevatorStart; i--) {
							extendFloor(i, draggingElevator->position.x, draggingElevator->getRect().maxX());
						}
					}
				}

				// Update PathFinder map
				gameMap.handleElevatorResize(draggingElevator, draggingElevatorLower, draggingElevatorStart);
				
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
	morningSound.setLoop(time.hour < 8);
	
	//Constrain the POI.
	double2 halfsize((win.getView().getSize().x)*0.5*zoom, (win.getView().getSize().y)*0.5*zoom);
	poi.y = std::max<double>(std::min<double>(poi.y, 360*12 - halfsize.y), -360 + halfsize.y);
	
	//Adust the camera.
	sf::FloatRect view;
	view.left   = round(poi.x - halfsize.x);
	view.top    = round(-poi.y - halfsize.y);
	view.width  = halfsize.x*2;
	view.height = halfsize.y*2;
	sf::View cameraView(view);
	win.setView(cameraView);
	//sf::FloatRect view = cameraView.GetRect();
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
	
	//Draw the sky and decorations.
	win.Draw(sky);
	win.Draw(decorations);
	
	//Draw the items that are in view.
	Item::Item * previousItemBelowCursor = itemBelowCursor;
	itemBelowCursor = NULL;

	//Draw floor items first
	for (ItemSet::iterator i = itemsByType["floor"].begin(); i != itemsByType["floor"].end(); i++) {
		const sf::Vector2f & vp = (*i)->GetPosition();
		const sf::Vector2f & vs = (*i)->GetSize();
		if (vp.x+vs.x >= view.Left && vp.x <= view.Right && vp.y >= view.Top && vp.y-vs.y <= view.Bottom) {
			win.Draw(**i);
			if ((*i)->getMouseRegion().containsPoint(double2(mp.x, mp.y))) itemBelowCursor = *i;
		}
	}

	//Draw the remaining items in the building
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

	//Play background sounds at a regular interval.
	const double backgroundSoundPeriod = 0.5 * Time::kBaseSpeed;
	if (floor(time.absolute / backgroundSoundPeriod) != floor((time.absolute - time.dta) / backgroundSoundPeriod)) {
		playRandomBackgroundSound();
	}
	
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
	itemsByType[item->prototype->id].insert(item);

	if (item->canHaulPeople()) {
		itemsByType["canHaulPeople"].insert(item);
		if (item->isElevator()) itemsByType["elevator"].insert(item);
		else itemsByType["stairlike"].insert(item);
	}

	if (item->prototype->icon == 1) {
		// Add floor item
		Item::Floor * f = (Item::Floor *) item;
		if (floorItems.count(item->position.y) != 0) {
			// Replace existing floor item with newly created floor item
			Item::Floor * existing_f = floorItems[item->position.y];
			if (existing_f->size.x >= f->size.x) {
				f->position = existing_f->position;
				f->size = existing_f->size;
			} else {
				std::multiset<int> &existing_f_i = existing_f->interval;
				existing_f_i.erase(existing_f->position.x);
				existing_f_i.erase(existing_f->getRect().maxX());
				existing_f_i.insert(f->position.x);
				existing_f_i.insert(f->getRect().maxX());
			}
			
			f->interval = existing_f->interval;
			removeItem(existing_f);
		}
		
		floorItems[item->position.y] = f;
		decorations.updateFloor(item->position.y);
	} else {
		for (int i = 0; i < item->size.y; i++) {
			itemsByFloor[item->position.y + i].insert(item);
			
			if (floorItems.count(item->position.y + i) == 0) {
				// This is necessary in case of loading a save game where the floor item is not loaded before the building item on that floor.
				int minX = item->position.x;
				int maxX = item->getRect().maxX();
				Item::Floor * f = (Item::Floor *) itemFactory.make(itemFactory.prototypesById["floor"], int2(minX,item->position.y + i));
				f->interval.erase(f->getRect().maxX());
				f->size.x = maxX - minX;
				f->interval.insert(f->getRect().maxX());
				f->updateSprite();
				addItem(f);
			}
			if (!item->canHaulPeople()) {
				std::multiset<int> &interval = floorItems[item->position.y + i]->interval;
				interval.insert(item->position.x);
				interval.insert(item->getRect().maxX());
			}
		}
	}

	gameMap.addNode(MapNode::Point(item->position.x + item->size.x/2, item->position.y), item);
	decorations.updateCrane();
	if (item == metroStation) decorations.updateTracks();
}

void Game::removeItem(Item::Item * item)
{
	assert(item);
	items.erase(item);
	itemsByType[item->prototype->id].erase(item);

	if (item->canHaulPeople()) {
		itemsByType["canHaulPeople"].erase(item);
		if (item->isElevator()) itemsByType["elevator"].erase(item);
		else itemsByType["stairlike"].erase(item);
	}

	if (item->prototype->icon == 1) {
		// Remove floor item
		floorItems.erase(item->position.y);
		decorations.updateFloor(item->position.y);
	} else {
		for (int i = 0; i < item->size.y; i++) {
			itemsByFloor[item->position.y + i].erase(item);
			if (!item->canHaulPeople()) {
				std::multiset<int> &interval = floorItems[item->position.y + i]->interval;
				interval.erase(interval.find(item->position.x));
				interval.erase(interval.find(item->getRect().maxX()));
			}
		}
	}

	if (item == itemBelowCursor) itemBelowCursor = NULL;

	gameMap.removeNode(MapNode::Point(item->position.x + item->size.x/2, item->position.y), item);
	decorations.updateCrane();
	if (item->prototype->icon == 19) decorations.updateTracks(); // Technically, this should not happen as Metro Stations are not removable.
}

void Game::extendFloor(int floor, int minX, int maxX) {
	if (floorItems.count(floor) != 0) {
		// Look for existing floor to extend
		Item::Floor * f = floorItems[floor];
		std::multiset<int> &interval = f->interval;
		interval.erase(interval.find(f->position.x));
		interval.erase(interval.find(f->getRect().maxX()));
		gameMap.removeNode(MapNode::Point(f->position.x + f->size.x/2, f->position.y), f);
		float diff_left = 0;
		if (minX < f->position.x) {
			diff_left = f->position.x - minX;
			f->size.x += diff_left;
			f->setPosition(int2(minX, floor));
		}

		float diff_right = 0;
		diff_right = maxX - f->getRect().maxX();
		if (diff_right < 0) diff_right = 0;
		f->size.x += diff_right;

		f->updateSprite();
		interval.insert(f->position.x);
		interval.insert(f->getRect().maxX());
		gameMap.addNode(MapNode::Point(f->position.x + f->size.x/2, f->position.y), f);
		if (diff_left + diff_right > 0) {
			decorations.updateFloor(f->position.y);
			transferFunds(-f->prototype->price * (diff_left + diff_right));
			playOnce("simtower/construction/flexible");
		}
	} else {
		// Otherwise construct a new floor
		Item::Floor * f = (Item::Floor *) itemFactory.make(itemFactory.prototypesById["floor"], int2(minX,floor));
		f->interval.erase(f->getRect().maxX());
		f->size.x = maxX - minX;
		f->interval.insert(f->getRect().maxX());
		f->updateSprite();
		addItem(f);
		transferFunds(-f->prototype->price);
		playOnce("simtower/construction/normal");
	}
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
		if (soundPlayTimes[sound] > time.absolute - 0.25 * Time::kBaseSpeed) {
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

/** Randomly picks one of the items in view and asks it for a background sound that it would like
 * to have played back. */
void Game::playRandomBackgroundSound()
{
	sf::RenderWindow &win = app.window;
	sf::FloatRect view = win.GetView().GetRect();

	//Pick a random value between 0 and the screen area.
	double r = (double)rand() / RAND_MAX * (view.Right-view.Left) * (view.Bottom-view.Top);

	//Iterate through the items that are in view, subtracting each item's area from the random
	//value until it drops below 0. That item will be given the chance to play the sound. This
	//effectively picks a random item in view, weighted by the item areas.
	Item::Item *pick = NULL;
	for (ItemSet::iterator i = items.begin(); i != items.end(); i++) {
		if ((*i)->layer != 0) continue;
		const sf::Vector2f & vp = (*i)->GetPosition();
		const sf::Vector2f & vs = (*i)->GetSize();
		if (vp.x+vs.x >= view.Left && vp.x <= view.Right && vp.y >= view.Top && vp.y-vs.y <= view.Bottom) {
			int area = vs.x * vs.y;
			r -= area;
			if (r <= 0) {
				pick = *i;
				break;
			}
		}
	}

	//Ask the picked item to play the sound.
	if (pick) {
		LOG(DEBUG, "Playing random background sound of %s", pick->desc().c_str());
		Path path = pick->getRandomBackgroundSoundPath();
		if (!path.str().empty()) {
			LOG(DEBUG, "-> sound is %s", path.c_str());
			playOnce(path);
		}
	}
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
Route Game::findRoute(Item::Item * start, Item::Item * destination, bool serviceRoute)
{
	MapNode::Point start_point(start->position.x + start->size.x/2, start->position.y + start->prototype->exit_offset);
	MapNode::Point end_point(destination->position.x + destination->size.x/2, destination->position.y + destination->prototype->entrance_offset);
	
	// For the special case of Metro, passengers can enter/exit from the middle floor if their destination is on that floor
	if (start->prototype->icon == 19 && (end_point.y == start_point.y - 1))
		start_point.y = end_point.y;
	else if (destination->prototype->icon == 19 && (start_point.y == end_point.y - 1))
		end_point.y = start_point.y;

	MapNode *start_mapnode = gameMap.findNode(start_point, start);
	MapNode *destination_mapnode = gameMap.findNode(end_point, destination);
	return pathFinder.findRoute(start_mapnode, destination_mapnode, start, destination, serviceRoute);
}
