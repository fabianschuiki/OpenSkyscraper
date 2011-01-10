#include "game.h"

//#include "../gui/gui.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

GameScene::GameScene(Tower * tower, Engine::EngineCore * engine) :
Engine::Scene(engine), tower(tower)
{	
	/*isDraggingConstruction = false;
	
	tool = ToolboxWindow::kInspectTool;
	
	//DEBUG: Setup the debugging construction tool visualization sprite
	debugConstructionToolSprite.texture = Texture::named("simtower/ui/toolbox/construction/normal");
	debugConstructionToolSprite.rect = recti(0, 0, 32, 32);
	debugConstructionToolSprite.textureRect.size.x = 1.0 / 8;
	debugConstructionToolSprite.textureRect.size.y = 1.0 / 4;
	debugItemType = Item::kLobbyType;*/
	
	//Initialize the GUI
	//gui = new GUI(this);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

void GameScene::advance(double dt)
{
	//Update the visible rect
	int2 canvasSize = Engine::Video::getCurrent()->currentMode.resolution;
	visibleRect.origin = POI - canvasSize / 2;
	visibleRect.size = canvasSize;
	
	//Calculate the mouse location
	previousWorldMouse = worldMouse;
	int2 mouse;
	SDL_GetMouseState(&mouse.x, &mouse.y);
	worldMouse.x = visibleRect.minX() + mouse.x;
	worldMouse.y = visibleRect.maxY() - 1 - mouse.y;
	mouseMoved = (worldMouse != previousWorldMouse);
	
	//Update the construction if the mouse was moved
	/*if (mouseMoved) {
		updateConstruction();
	}*/
	
	//Calculate the visible cells rect
	visibleCells = tower->convertWorldToCellRect(visibleRect);
	
	//Advance the tower
	tower->advance(dt);
	
	//Find the items to draw
	visibleFacilities.clear();
	visibleTransports.clear();
	for (int x = visibleCells.minX(); x < visibleCells.maxX(); x++) {
		for (int y = visibleCells.minY(); y < visibleCells.maxY(); y++) {
			Tower::Cell * cell = tower->getCell(int2(x, y));
			if (!cell) continue;
			
			if (cell->facility) visibleFacilities.insert(cell->facility);
			if (cell->transport) visibleTransports.insert(cell->transport);
		}
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark State
//----------------------------------------------------------------------------------------------------

void GameScene::update()
{
	//Update the tower
	tower->update();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

void GameScene::draw()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	
	//Translate modelview matrix so that the POI is centered on screen
	glTranslated(-visibleRect.origin.x, -visibleRect.origin.y, 0);
	
	//Draw the tower
	tower->draw(visibleRect);
	
	/*//Draw the construction template
	rectd worldConstructionRect = tower->convertCellToWorldRect(constructionTemplate);
	worldConstructionRect.inset(0.5, 0.5);
	glColor3f(1, 1, 1);
	Engine::Texture::unbind();
	glBegin(GL_LINE_STRIP);
	glVertex2d(worldConstructionRect.minX(), worldConstructionRect.minY());
	glVertex2d(worldConstructionRect.maxX(), worldConstructionRect.minY());
	glVertex2d(worldConstructionRect.maxX(), worldConstructionRect.maxY());
	glVertex2d(worldConstructionRect.minX(), worldConstructionRect.maxY());
	glVertex2d(worldConstructionRect.minX(), worldConstructionRect.minY());
	glEnd();*/
	
	//Draw a red cross at the origin of the world
	glBindTexture(GL_TEXTURE_RECTANGLE_EXT, 0);
	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	glVertex2f(-9.5, 0.5);
	glVertex2f(10.5, 0.5);
	glVertex2f(0.5, -9.5);
	glVertex2f(0.5, 10.5);
	glEnd();
	
	//Draw a green cross where the mouse is
	glColor3f(0, 1, 0);
	glBegin(GL_LINES);
	glVertex2f(worldMouse.x - 9.5, worldMouse.y + 0.5);
	glVertex2f(worldMouse.x + 10.5, worldMouse.y + 0.5);
	glVertex2f(worldMouse.x + 0.5, worldMouse.y - 9.5);
	glVertex2f(worldMouse.x + 0.5, worldMouse.y + 10.5);
	glEnd();
	
	//Draw the GUI
	glLoadIdentity();
	//drawGUI();
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
#pragma mark OpenGL State
//----------------------------------------------------------------------------------------------------

void GameScene::didMoveOnScreen()
{
	//Engine::shared()->audioTask.playSound(Sound::named("simtower/#4E20"), SoundEffect::kTopLayer);
	
	Scene::didMoveOnScreen();
	
	glEnable(GL_TEXTURE_RECTANGLE_EXT);
	glEnable(GL_BLEND);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//setConstructionTool(kLobbyType);
	
	/*TwBar * myBar = TwNewBar("NameOfMyTweakBar");
	TwAddButton(myBar, "Hello, World!", NULL, NULL, NULL);*/
}

void GameScene::willMoveOffScreen()
{	
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_RECTANGLE_EXT);
	
	Scene::willMoveOffScreen();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Events
//----------------------------------------------------------------------------------------------------

/*bool GameScene::handleEvent(CoreEvent * event)
{
	if (toolboxWindow.handleEvent((GameEvent *)event)) return true;
	if (controlWindow.handleEvent((GameEvent *)event)) return true;
	if (tower && tower->handleEvent((GameEvent *)event)) return true;
	return Scene::handleEvent(event);
}

bool GameScene::eventKeyDown(SDL_Event * event)
{
	double factor = (event->key.keysym.mod & KMOD_SHIFT ? 10 : 1);
	switch (event->key.keysym.sym) {
		case SDLK_UP:		POI.y += 10 * factor; return true; break;
		case SDLK_DOWN:		POI.y -= 10 * factor; return true; break;
			
			//DEBUG: Enable construction tool change using page up/down keys
		case SDLK_PAGEUP:	setConstructionTool((Item::Type)(++debugItemType)); return true; break;
		case SDLK_PAGEDOWN:	setConstructionTool((Item::Type)(--debugItemType)); return true; break;
	}
	switch (event->key.keysym.unicode) {
			//DEBUG: Change tower rating
		case '1':	tower->rating = 1; return true; break;
		case '2':	tower->rating = 2; return true; break;
		case '3':	tower->rating = 3; return true; break;
		case '4':	tower->rating = 4; return true; break;
		case '5':	tower->rating = 5; return true; break;
		case '6':	tower->rating = 6; return true; break;
			
			//DEBUG: Change time
		case 'h':	tower->time += 1; return true; break;
		case ' ':	tower->time += 0.2; return true; break;
		case 'a':	tower->debugSpeed--; return true; break;
		case 's':	tower->debugSpeed++; return true; break;
			
			//DEBUG: Build debug tower
		case 'd':	buildDebugTower(); return true; break;
			
			//DEBUG: Pause
		case 'p':	tower->paused = !tower->paused; return true; break;
	}
	return false;
}

bool GameScene::eventMouseDown(SDL_Event * event)
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

/*void GameScene::buildDebugTower()
{
	Engine::shared()->audioTask.disableSoundEffects();
	
	//Lobby
	tower->constructFlexibleWidthItem(Item::descriptorForItemType(Item::kLobbyType),
									  recti(-50, 0, 0, 1), recti(50, 0, 0, 1));
	
	//Hotels
	for (int y = 1; y < 6; y++) {
		for (int x = 0; x < 2; x++) {
			tower->constructItem(Item::descriptorForItemType(Item::kSingleRoomType),
								 recti(x * 4, y, 4, 1));
			tower->constructItem(Item::descriptorForItemType(Item::kSingleRoomType),
								 recti((-x - 1) * 4, y, 4, 1));
		}
		for (int x = 0; x < 6; x++) {
			tower->constructItem(Item::descriptorForItemType(Item::kDoubleRoomType),
								 recti(8 + x * 6, y, 6, 1));
			tower->constructItem(Item::descriptorForItemType(Item::kDoubleRoomType),
								 recti(-8 + (-x - 1) * 6, y, 6, 1));
		}
		tower->constructItem(Item::descriptorForItemType(Item::kEscalatorType),
							 recti(-4, y - 1, 8, 2));
	}
	
	//Housekeeping
	tower->constructItem(Item::descriptorForItemType(Item::kHousekeepingType),
						 recti(-7, 6, 15, 1));
	tower->constructItem(Item::descriptorForItemType(Item::kEscalatorType),
						 recti(-4, 5, 8, 2));
	
	//Offices
	for (int y = 1; y < 15; y++) {
		for (int x = 0; x < 1; x++) {
			tower->constructItem(Item::descriptorForItemType(Item::kOfficeType),
								 recti(x * 9, y, 9, 1));
			tower->constructItem(Item::descriptorForItemType(Item::kOfficeType),
								 recti((-x - 1) * 9, y, 9, 1));
		}
	}
	tower->constructFlexibleWidthItem(Item::descriptorForItemType(Item::kLobbyType),
									  recti(-9, 15, 0, 1), recti(9, 15, 0, 1));
	
	//Stairs
	for (int y = 0; y < 1; y++) {
		tower->constructItem(Item::descriptorForItemType(Item::kStairsType),
							 recti(-4, y, 8, 2));
	 }
	//Engine::shared()->audioTask.enableSoundEffects();
}*/
