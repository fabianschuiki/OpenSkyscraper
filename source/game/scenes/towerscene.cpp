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
	tower->renderBackground(visibleRect);
	
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
}

void TowerScene::onMoveOffScreen()
{
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
		constructFlexibleWidthItem(constructionItemDescriptor, constructionTemplate, previousConstructionTemplate);
		previousConstructionTemplate = constructionTemplate;
	}
}

void TowerScene::startConstruction()
{	
	//Is this a draggable item like the lobby or a floor?
	if (constructionItemDescriptor->attributes & Item::kFlexibleWidthAttribute) {		
		//Do the initial build
		previousConstructionTemplate = constructionTemplate;
		bool success = constructFlexibleWidthItem(constructionItemDescriptor, constructionTemplate, previousConstructionTemplate);
		
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
}

bool TowerScene::constructFlexibleWidthItem(Item::Descriptor * descriptor, recti currentRect, recti previousRect)
{
	OSSObjectLog << "attempting construction of " << descriptor->type << " from " << previousRect.description() << " to " << currentRect.description() << std::endl;
	
	//Check whether the basic construction limitations are given
	if ((descriptor->attributes & Item::kEvery15thFloorAttribute) && (currentRect.origin.y % 15) != 0) {
		OSSObjectError << "failed because only every 15th floor" << std::endl;
		return false;
	}
	
	//Calculate the union rect for the current and previous rect
	recti rect = currentRect.unionRect(previousRect);
	OSSObjectLog << "scanning rect " << rect.description() << "..." << std::endl;
	
	//Decide where and in which direction to start scanning
	int scanStep = (previousRect.origin.x < currentRect.origin.x ? 1 : -1);
	int2 scanOrigin(scanStep == 1 ? rect.minX() : rect.maxX() - 1, rect.minY());
	
	//Scan the facility entries until we find the first occupied or impossible slot
	int i;
	unsigned int floorCellsRequired = 0;
	unsigned int itemCellsRequired = 0;
	std::set<Item *> facilitiesHit;
	for (i = 0; i < rect.size.x; i++) {
		int2 pos = scanOrigin + int2(i * scanStep, 0);
		int2 posBelow = pos - int2(0, 1);
		Tower::Cell * cell = tower->getCell(pos);
		Tower::Cell * cellBelow = tower->getCell(posBelow);
		
		//The cell below must have a built facility in some way
		if (rect.origin.y > 0 && (!cellBelow || cellBelow->facility == 0)) {
			OSSObjectError << "cell below invalid" << std::endl;
			break;
		}
		
		//The cell must be either empty, a floor or the same as the item
		if (cell) {
			//Check whether the cell has a facility that is not the floor facility (1)
			if (cell->facility > 1) {
				//Fetch the facility already placed in this cell
				Item * facility = tower->facilityItems[cell->facility];
				if (facility) {
					//If it is valid, check whether it is not of our type, which would be a dealbreaker
					if (facility->descriptor->type != descriptor->type) {
						OSSObjectError << "floor is already occupied by something else" << std::endl;
						break;
					} else {
						//Note the facility we just hit, since we need to remove it later and expand
						//the item we're building to cover this facility's area too.
						facilitiesHit.insert(facility);
					}
				}
			} else {
				//Since there's no facility there yet we need to build one
				itemCellsRequired++;
			}
			
			//If there's no facility in this cell, we need to build a floor
			if (cell->facility == 0)
				floorCellsRequired++;
		} else {
			//Since there's nothing here we need to build both the floor and the item
			floorCellsRequired++;
			itemCellsRequired++;
		}
	}
	
	//Calculate the last valid position
	int2 lastValidPosition = scanOrigin;
	lastValidPosition.x += ((i - 1) * scanStep);
	
	OSSObjectLog << "scan started at " << scanOrigin.description() << " and lastet for " << i << " cells" << std::endl;
	OSSObjectLog << floorCellsRequired << " floor cells required, " << itemCellsRequired << " item cells required" << std::endl;
	
	//We won't continue if the number of valid cells is smaller than the minUnit size
	if (i < descriptor->minUnit.x) return false;
	
	//Calculate the cost of this construction
	double cost = floorCellsRequired * Item::descriptorForItemType(Item::kFloorType)->price;
	cost += itemCellsRequired * descriptor->price;
	
	//Check whether the money is available...
	OSSObjectLog << "construction costs " << cost << std::endl;
	
	//Calculate the valid rect
	recti validRect(std::min<int>(scanOrigin.x, lastValidPosition.x), scanOrigin.y, abs(scanOrigin.x - lastValidPosition.x) + 1, 1);
	OSSObjectLog << "the item's valid rect is " << validRect.description() << std::endl;
	
	//Fetch the item ID we will assign the new item
	unsigned int itemID = tower->nextItemID();
	
	//Add adjacent facilities of the same type to the list of hit facilities, so they get collapsed
	Tower::Cell * cellLeft = tower->getCell(int2(validRect.minX() - 1, validRect.minY()));
	Tower::Cell * cellRight = tower->getCell(int2(validRect.maxX(), validRect.minY()));
	if (cellLeft && cellLeft->facility > 1) {
		Item * facilityLeft = tower->facilityItems[cellLeft->facility];
		if (facilityLeft->descriptor->type == descriptor->type) {
			OSSObjectLog << "adding adjacent facility " << facilityLeft->itemID << " on the left" << std::endl;
			facilitiesHit.insert(facilityLeft);
		}
	}
	if (cellRight && cellRight->facility > 1) {
		Item * facilityRight = tower->facilityItems[cellRight->facility];
		if (facilityRight->descriptor->type == descriptor->type) {
			OSSObjectLog << "adding adjacent facility " << facilityRight->itemID << " on the right" << std::endl;
			facilitiesHit.insert(facilityRight);
		}
	}
	
	//Calculate the item rect which actually is a union of the validRect and all the hit facilities.
	//We also need to get rid of the hit facilities themselves
	recti itemRect = validRect;
	std::set<Item *>::iterator it;
	for (it = facilitiesHit.begin(); it != facilitiesHit.end(); it++) {
		itemRect.unify((*it)->rect);
		tower->facilityItems.erase((*it)->itemID);
	}
	OSSObjectLog << "the item rect is " << itemRect.description() << std::endl;
	
	//Make the cells the item covers point at it
	for (int x = itemRect.minX(); x < itemRect.maxX(); x++) {
		Tower::Cell * cell = tower->getCell(int2(x, itemRect.origin.y), true);
		cell->facility = itemID;
	}
	
	//Create the new item and add it to the tower's facilities
	Item * item = Item::createNew(descriptor, itemRect, itemID);
	tower->facilityItems[itemID] = item;
	
	return true;
}
