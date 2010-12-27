#include "towerscene.h"
#include "../../classes.h"

using namespace OSS;


//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

void TowerScene::advance(double dt)
{
	//Update the visible rect
	int2 canvasSize = OpenGLCanvas::shared()->currentMode.resolution;
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
	if (mouseMoved) {
		updateConstruction();
	}
	
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
			
			visibleFacilities.insert(cell->facility);
			visibleTransports.insert(cell->transport);
		}
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Rendering
//----------------------------------------------------------------------------------------------------

void TowerScene::render()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	
	//Translate modelview matrix so that the POI is centered on screen
	glTranslated(-visibleRect.origin.x, -visibleRect.origin.y, 0);
	
	//Render the background
	renderBackground();
	
	//Render the items
	renderFacilities();
	renderTransports();
	
	//Draw the construction template
	rectd worldConstructionRect = tower->convertCellToWorldRect(constructionTemplate);
	worldConstructionRect.inset(0.5, 0.5);
	glColor3f(1, 1, 1);
	Texture::unbind();
	glBegin(GL_LINE_STRIP);
	glVertex2d(worldConstructionRect.minX(), worldConstructionRect.minY());
	glVertex2d(worldConstructionRect.maxX(), worldConstructionRect.minY());
	glVertex2d(worldConstructionRect.maxX(), worldConstructionRect.maxY());
	glVertex2d(worldConstructionRect.minX(), worldConstructionRect.maxY());
	glVertex2d(worldConstructionRect.minX(), worldConstructionRect.minY());
	glEnd();
	
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
	
	OpenGLCanvas::shared()->swapBuffers();
}

void TowerScene::renderBackground()
{
	//Render the background
	tower->renderBackground(visibleRect);
}

void TowerScene::renderFacilities()
{
	//OSSObjectLog << "rendering " << visibleFacilities.size() << " facilities..." << std::endl;
	std::set<unsigned int>::iterator it;
	for (it = visibleFacilities.begin(); it != visibleFacilities.end(); it++) {
		Item * facility = tower->facilityItems[*it];
		assert(facility != NULL);
		facility->worldRect = tower->convertCellToWorldRect(facility->rect);
		facility->draw(visibleRect);
	}
}

void TowerScene::renderTransports()
{
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark OpenGL State
//----------------------------------------------------------------------------------------------------

void TowerScene::onMoveOnScreen()
{
	Scene::onMoveOnScreen();
	
	glEnable(GL_TEXTURE_RECTANGLE_EXT);
	glEnable(GL_BLEND);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	constructionItemDescriptor = Item::descriptorForItemType(Item::kLobbyType);
	
	tower->onMoveOnScreen();
}

void TowerScene::onMoveOffScreen()
{
	tower->onMoveOffScreen();
	
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_RECTANGLE_EXT);
	
	Scene::onMoveOffScreen();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Events
//----------------------------------------------------------------------------------------------------

bool TowerScene::handleEvent(CoreEvent * event)
{
	if (tower && tower->handleEvent(event)) return true;
	return Scene::handleEvent(event);
}

bool TowerScene::eventKeyDown(SDL_Event * event)
{
	double factor = (event->key.keysym.mod & KMOD_SHIFT ? 10 : 1);
	switch (event->key.keysym.sym) {
		case SDLK_UP:		POI.y += 10 * factor; return true; break;
		case SDLK_DOWN:		POI.y -= 10 * factor; return true; break;
	}
	return false;
}

bool TowerScene::eventMouseDown(SDL_Event * event)
{
	OSSObjectLog << event->button.x << " x " << event->button.y << std::endl;
	switch (event->button.button) {
		case SDL_BUTTON_WHEELUP:	POI.y += 10; return true; break;
		case SDL_BUTTON_WHEELDOWN:	POI.y -= 10; return true; break;
			
		case SDL_BUTTON_LEFT: startConstruction(); return true; break;
	}
	return false;
}

bool TowerScene::eventMouseUp(SDL_Event * event)
{
	switch (event->button.button) {
		case SDL_BUTTON_LEFT: endConstruction(); return true; break;
	}
	return false;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

void TowerScene::updateConstruction()
{
	//Update the construction template
	constructionTemplate.size = constructionItemDescriptor->minUnit;
	constructionTemplate.origin.x = round(worldMouse.x / tower->cellSize.x - (double)constructionTemplate.size.x / 2);
	if (!isDraggingConstruction)
		constructionTemplate.origin.y = round(worldMouse.y / tower->cellSize.y - (double)constructionTemplate.size.y / 2);
	
	//If we're dragging a construction at the moment, construct the next segment
	if (isDraggingConstruction && (constructionTemplate != previousConstructionTemplate)) {
		tower->constructFlexibleWidthItem(constructionItemDescriptor, constructionTemplate, previousConstructionTemplate);
		previousConstructionTemplate = constructionTemplate;
	}
}

void TowerScene::startConstruction()
{	
	//Is this a draggable item like the lobby or a floor?
	if (constructionItemDescriptor->attributes & Item::kFlexibleWidthAttribute) {
		//Disable the constructions
		tower->setConstructionsHalted(true);
		
		//Do the initial build
		previousConstructionTemplate = constructionTemplate;
		bool success = tower->constructFlexibleWidthItem(constructionItemDescriptor,
														 constructionTemplate,
														 previousConstructionTemplate);
		
		//If the build was successful, start the dragging
		if (success)
			isDraggingConstruction = true;
	} else {
		//Simply build the item
		//constructItem(constructionItemDescriptor, constructionTemplate);
	}
}

void TowerScene::endConstruction()
{
	isDraggingConstruction = false;
	
	//Resume the constructions
	tower->setConstructionsHalted(false);
}
