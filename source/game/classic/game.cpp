#include "game.h"

#include "items/item.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

GameScene::GameScene(Tower * tower, Engine * engine) : Scene(engine), Classic::Responder(this),
tower(tower), updateVisibleRectIfNeeded(this, &GameScene::updateVisibleRect, &updateIfNeeded)
{	
	//Initialize the GUI
	ui = new GameUI(this);
	
	//Setup some default POI location
	setPOI(int2(0, 200));
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Camera
//----------------------------------------------------------------------------------------------------

const double2 & GameScene::getPOI()
{
	return POI;
}

void GameScene::setPOI(double2 p)
{
	if (POI != p) {
		
		//Make sure the visible frame does not exceed the tower's bounds.
		rectd bounds = tower->structure->getWorldEnvironmentRect();
		p.x = std::max<double>(bounds.minX() + visibleRect.size.x / 2, p.x);
		p.x = std::min<double>(bounds.maxX() - visibleRect.size.x / 2, p.x);
		p.y = std::max<double>(bounds.minY() + visibleRect.size.y / 2, p.y);
		p.y = std::min<double>(bounds.maxY() - visibleRect.size.y / 2, p.y);
		
		//Set the point.
		POI = p;
		updateVisibleRectIfNeeded.setNeeded();
	}
}



const rectd & GameScene::getVisibleRect()
{
	return visibleRect;
}

void GameScene::setVisibleRect(rectd rect)
{
	if (visibleRect != rect) {
		visibleRect = rect;
	}
}



double2 GameScene::windowToWorld(double2 v, bool flipped)
{
	if (flipped)
		return (v * double2(1, -1) + getVisibleRect().minXmaxY());
	else
		return (v + getVisibleRect().minXminY());
}

double2 GameScene::windowToWorld(double2 v)
{
	return windowToWorld(v, false);
}

rectd GameScene::windowToWorld(rectd v, bool flipped)
{
	rectd r(windowToWorld(v.origin, flipped), v.size);
	if (flipped)
		r.origin -= r.size;
	return r;
}

rectd GameScene::windowToWorld(rectd v)
{
	return windowToWorld(v, false);
}



double2 GameScene::worldToWindow(double2 v, bool flipped)
{
	if (flipped)
		return (v * double2(1, -1) - getVisibleRect().minXmaxY());
	else
		return (v - getVisibleRect().minXminY());
}

double2 GameScene::worldToWindow(double2 v)
{
	return worldToWindow(v, false);
}

rectd GameScene::worldToWindow(rectd v, bool flipped)
{
	rectd r(worldToWindow(v.origin, flipped), v.size);
	if (flipped)
		r.origin += r.size;
	return r;
}

rectd GameScene::worldToWindow(rectd v)
{
	return worldToWindow(v, false);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

void GameScene::advance(double dt)
{
	//Advance the GUI
	ui->advance(dt);
	
	//Advance the tower
	tower->advance(dt);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark State
//----------------------------------------------------------------------------------------------------

void GameScene::update()
{
	//Update our state where required
	updateVisibleRectIfNeeded();
	
	//Update the GUI
	ui->updateIfNeeded();
	
	//Update the tower
	tower->updateIfNeeded();
}

void GameScene::updateVisibleRect()
{
	rectd rect(getPOI(), Video::shared()->currentMode.resolution);
	rect.origin -= rect.size / 2;
	setVisibleRect(rect);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

void GameScene::draw(rectd dirtyRect)
{
	//Black screen to start off
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	
	//Translate modelview matrix so that the POI is centered on screen
	double2 origin = getVisibleRect().origin;
	glTranslated(-origin.x, -origin.y, 0);
	
	//Draw the tower
	tower->draw(visibleRect);
	
	//Draw a red cross at the origin of the world
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, 0);
	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	glVertex2f(-9.5, 0.5);
	glVertex2f(10.5, 0.5);
	glVertex2f(0.5, -9.5);
	glVertex2f(0.5, 10.5);
	glEnd();
	
	//Draw a green cross where the mouse is
	int2 mouse;
	SDL_GetMouseState(&mouse.x, &mouse.y);
	double2 worldMouse = windowToWorld(mouse, true);
	glColor3f(0, 1, 0);
	glBegin(GL_LINES);
	glVertex2f(worldMouse.x - 9.5, worldMouse.y + 0.5);
	glVertex2f(worldMouse.x + 10.5, worldMouse.y + 0.5);
	glVertex2f(worldMouse.x + 0.5, worldMouse.y - 9.5);
	glVertex2f(worldMouse.x + 0.5, worldMouse.y + 10.5);
	glEnd();
	
	//Calculate the visible rect of the UI
	rectd visibleRectUI;
	visibleRectUI.size = Video::shared()->currentMode.resolution;
	
	//Draw the UI
	glLoadIdentity();
	ui->draw(visibleRectUI);
}

/*void GameScene::renderFacilities()
{
	//OSSObjectLog << "rendering " << visibleFacilities.size() << " facilities..." << std::endl;
	std::set<unsigned int>::iterator it;
	for (it = visibleFacilities.begin(); it != visibleFacilities.end(); it++) {
		Item * facility = tower->facilityItems[*it];
		assert(facility != NULL);
		facility->draw(visibleRect);
	}
}

void GameScene::renderTransports()
{
	//OSSObjectLog << "rendering " << visibleFacilities.size() << " facilities..." << std::endl;
	std::set<unsigned int>::iterator it;
	for (it = visibleTransports.begin(); it != visibleTransports.end(); it++) {
		Item * transport = tower->transportItems[*it];
		assert(transport != NULL);
		transport->draw(visibleRect);
	}
}

void GameScene::renderGUI()
{
	//DEBUG: draw the current construction tool
	debugConstructionToolSprite.draw(visibleRect);
	
	//Draw the control window
	controlWindow.draw(visibleRect);
	
	//Draw the toolbox window
	toolboxWindow.draw(visibleRect);
}*/





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Presentation
//----------------------------------------------------------------------------------------------------

void GameScene::didMoveOnScreen()
{
	Scene::didMoveOnScreen();
	
	glEnable(GL_TEXTURE_RECTANGLE_ARB);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GameScene::willMoveOffScreen()
{	
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_RECTANGLE_ARB);
	
	Scene::willMoveOffScreen();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Events
//----------------------------------------------------------------------------------------------------

bool GameScene::sendEventToNextResponders(OSS::Event * event)
{
	if (ui && ui->sendEvent(event)) return true;
	if (tower && tower->sendEvent(event)) return true;
	return Scene::sendEventToNextResponders(event);
}

bool GameScene::eventScrollWheel(ScrollWheelEvent * event)
{
	//Move the POI accordingly
	setPOI(getPOI() + event->displacement * 10);
	return true;
}

bool GameScene::eventKeyDown(KeyEvent * event)
{
	//double factor = (event->key.keysym.mod & KMOD_SHIFT ? 10 : 1);
	switch (event->code) {
		case SDLK_UP:		setPOI(getPOI() + double2(0, 100)); return true; break;
		case SDLK_DOWN:		setPOI(getPOI() - double2(0, 100)); return true; break;
	}
	switch (event->unicode) {
			//DEBUG: Change tower rating
		case '1':	tower->environment->setRating(1); return true; break;
		case '2':	tower->environment->setRating(2); return true; break;
		case '3':	tower->environment->setRating(3); return true; break;
		case '4':	tower->environment->setRating(4); return true; break;
		case '5':	tower->environment->setRating(5); return true; break;
		case '6':	tower->environment->setRating(6); return true; break;
			
			//DEBUG: Change time
		case 'h':	tower->time->setTime(tower->time->getTime() + 1); return true; break;
		case ' ':	tower->time->setTime(tower->time->getTime() + 0.2); return true; break;
		case 'a':	tower->time->debugSpeed--; return true; break;
		case 's':	tower->time->debugSpeed++; return true; break;
			
			//DEBUG: Pause
		case 'p':	tower->time->setPaused(!tower->time->isPaused()); return true; break;
			
			//DEBUG: Debugging tower
		case 'd':	buildDebugTower(); return true; break;
	}
	return false;
}

/*bool GameScene::eventMouseDown(SDL_Event * event)
{
	switch (event->button.button) {
		case SDL_BUTTON_WHEELUP:	POI.y += 10; return true; break;
		case SDL_BUTTON_WHEELDOWN:	POI.y -= 10; return true; break;
			
		case SDL_BUTTON_LEFT: {
			switch (tool) {
				case ToolboxWindow::kConstructionTool: {
					startConstruction();
					return true;
				} break;
			}
		} break;
	}
	return false;
}

bool GameScene::eventMouseUp(SDL_Event * event)
{
	switch (event->button.button) {
		case SDL_BUTTON_LEFT: {
			switch (tool) {
				case ToolboxWindow::kConstructionTool: {
					endConstruction();
					return true;
				} break;
			}
		} break;
	}
	return false;
}*/





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

/*void GameScene::updateConstruction()
{
	if (!constructionItemDescriptor)
		return;
	
	//Update the construction template
	constructionTemplate.size = constructionItemDescriptor->minUnit;
	if (!constructionTemplate.size.length())
		constructionTemplate.size = constructionItemDescriptor->cells;
	constructionTemplate.origin.x = round(worldMouse.x / tower->cellSize.x - (double)constructionTemplate.size.x / 2);
	if (!isDraggingConstruction)
		constructionTemplate.origin.y = round(worldMouse.y / tower->cellSize.y - (double)constructionTemplate.size.y / 2);
	
	//If we're dragging a construction at the moment, construct the next segment
	if (isDraggingConstruction && (constructionTemplate != previousConstructionTemplate)) {
		tower->constructFlexibleWidthItem(constructionItemDescriptor, constructionTemplate, previousConstructionTemplate);
		previousConstructionTemplate = constructionTemplate;
	}
}

void GameScene::startConstruction()
{	
	if (!constructionItemDescriptor)
		return;
	
	//Is this a draggable item like the lobby or a floor?
	bool success = false;
	if (constructionItemDescriptor->attributes & Item::kFlexibleWidthAttribute) {
		//Disable the constructions
		tower->setConstructionsHalted(true);
		
		//Do the initial build
		previousConstructionTemplate = constructionTemplate;
		success = tower->constructFlexibleWidthItem(constructionItemDescriptor,
														 constructionTemplate,
														 previousConstructionTemplate);
		
		//If the build was successful, start the dragging
		if (success)
			isDraggingConstruction = true;
	} else {
		//Simply build the item
		success = tower->constructItem(constructionItemDescriptor, constructionTemplate);
	}
	
	//Play the annoying "click" sound if the construction was impossible :)
	if (!success)
		Engine::shared()->audioTask.playSound(Sound::named("simtower/construction/impossible"),
											  SoundEffect::kTopLayer);
}

void GameScene::endConstruction()
{
	isDraggingConstruction = false;
	
	//Resume the constructions
	tower->setConstructionsHalted(false);
}*/





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Uncategorized
//----------------------------------------------------------------------------------------------------

/*ToolboxWindow::Tool GameScene::getTool()
{
	return tool;
}

void GameScene::setTool(ToolboxWindow::Tool tool)
{
	if (this->tool != tool) {
		this->tool = tool;
		toolboxWindow.updateButtons();
	}
}

void GameScene::setConstructionTool(Item::Type itemType)
{	
	setTool(ToolboxWindow::kConstructionTool);
	
	//Skip if the item didn't change
	if (constructionItemDescriptor && itemType == constructionItemDescriptor->type)
		return;
	
	//Store the item
	constructionItemDescriptor = Item::descriptorForItemType(itemType);
	
	//DEBUG: Change the icon shown by the debug construction tool sprite
	debugConstructionToolSprite.textureRect.origin.x = ((itemType - 1) % 8) * debugConstructionToolSprite.textureRect.size.x;
	debugConstructionToolSprite.textureRect.origin.y = (3 - (itemType - 1) / 8) * debugConstructionToolSprite.textureRect.size.y;
	
	toolboxWindow.updateItemButtons();
}

void GameScene::eventPrepare()
{
}

void GameScene::setTower(Tower * tower)
{
	this->tower = tower;
	controlWindow.tower = tower;
	toolboxWindow.updateButtons();
}*/

void GameScene::buildDebugTower()
{
	Audio::shared()->disableSoundEffects();
	
	//Lobby
	tower->structure->constructItem(Item::descriptorForItemType(kLobbyType),
									  recti(-50, 0, 0, 1), recti(50, 0, 0, 1));
	
	//Hotels
	for (int y = 1; y < 6; y++) {
		for (int x = 0; x < 2; x++) {
			tower->structure->constructItem(Item::descriptorForItemType(kSingleRoomType),
								 recti(x * 4, y, 4, 1));
			tower->structure->constructItem(Item::descriptorForItemType(kSingleRoomType),
								 recti((-x - 1) * 4, y, 4, 1));
		}
		for (int x = 0; x < 6; x++) {
			tower->structure->constructItem(Item::descriptorForItemType(kDoubleRoomType),
								 recti(8 + x * 6, y, 6, 1));
			tower->structure->constructItem(Item::descriptorForItemType(kDoubleRoomType),
								 recti(-8 + (-x - 1) * 6, y, 6, 1));
		}
		/*tower->structure->constructItem(Item::descriptorForItemType(kEscalatorType),
							 recti(-4, y - 1, 8, 2));*/
	}
	tower->structure->constructItem(Item::descriptorForItemType(kStandardElevatorType),
									recti(-10, 0, 4, 1));
	
	//Housekeeping
	tower->structure->constructItem(Item::descriptorForItemType(kHousekeepingType),
									recti(-10, 6, 15, 1));
	tower->structure->constructItem(Item::descriptorForItemType(kHousekeepingType),
									recti(5, 6, 15, 1));
	tower->structure->constructItem(Item::descriptorForItemType(kServiceElevatorType),
									recti(16, 0, 4, 1));
	
	/*//Offices
	for (int y = 1; y < 15; y++) {
		for (int x = 0; x < 1; x++) {
			tower->constructItem(Item::descriptorForItemType(Item::kOfficeType),
								 recti(x * 9, y, 9, 1));
			tower->constructItem(Item::descriptorForItemType(Item::kOfficeType),
								 recti((-x - 1) * 9, y, 9, 1));
		}
	}
	tower->constructFlexibleWidthItem(Item::descriptorForItemType(Item::kLobbyType),
									  recti(-9, 15, 0, 1), recti(9, 15, 0, 1));*/
	
	//Stairs
	/*for (int y = 0; y < 1; y++) {
		tower->structure->constructItem(Item::descriptorForItemType(kStairsType),
							 recti(-4, y, 8, 2));
	 }*/
	
	Audio::shared()->enableSoundEffects();
}
