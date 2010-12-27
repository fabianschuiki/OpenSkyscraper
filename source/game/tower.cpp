#include "tower.h"
#include "../classes.h"

using namespace OSS;


//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

Tower::Tower()
{
	ceilingHeight = 12;
	cellSize = int2(8, 24 + ceilingHeight);
	constructionsHalted = false;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Coordinates
//----------------------------------------------------------------------------------------------------

recti Tower::convertWorldToCellRect(rectd rect)
{
	int2 origin = convertWorldToCellCoordinates(rect.origin);
	int2 size = convertWorldToCellCoordinates(rect.origin + rect.size) - origin;
	return recti(origin, size);
}

rectd Tower::convertCellToWorldRect(recti rect)
{
	return rectd(convertCellToWorldCoordinates(rect.origin), convertCellToWorldCoordinates(rect.size));
}



int2 Tower::convertWorldToCellCoordinates(double2 coordinates)
{
	return int2(round(coordinates.x / cellSize.x), round(coordinates.y / cellSize.y));
}

double2 Tower::convertCellToWorldCoordinates(int2 coordinates)
{
	return double2(coordinates.x * cellSize.x, coordinates.y * cellSize.y);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Notifications
//----------------------------------------------------------------------------------------------------

void Tower::onMoveOnScreen()
{
	//Prepare the background
	prepareBackground();
}

void Tower::onMoveOffScreen()
{
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

void Tower::advance(double dt)
{
	//Advance the facilities
	for (ItemMap::iterator it = facilityItems.begin(); it != facilityItems.end(); it++) {
		Item * item = it->second;
		if (item)
			item->advance(dt);
	}
	
	//Advance the Transports
	for (ItemMap::iterator it = transportItems.begin(); it != transportItems.end(); it++) {
		Item * item = it->second;
		if (item)
			item->advance(dt);
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Rendering
//----------------------------------------------------------------------------------------------------

void Tower::prepareBackground()
{
	//Initialize the ground sprite
	groundSprite = new Sprite;
	groundSprite->texture = Texture::named("simtower/049.bmp");
	groundSprite->rect = rectd(-800, -360, 1600, 360);
	groundSprite->textureMode = Sprite::kRepeatTextureMode;
	groundSprite->autoTexRectX = true;
	groundSprite->autoTexRectY = true;
	
	//Initialize the sky sprites
	for (int i = 0; i < 10; i++) {	
		char n[16];
		sprintf(n, "simtower/%03i.bmp", i + 50);
		skySprites[i] = new Sprite;
		skySprites[i]->texture = Texture::named(n);
		skySprites[i]->rect = rectd(-800, i * 360, 1600, 360);
		skySprites[i]->textureMode = Sprite::kRepeatTextureMode;
		skySprites[i]->autoTexRectX = true;
	}
	
	//Initialize the city sprite
	citySprite = new Sprite;
	citySprite->texture = Texture::named("simtower/065.bmp");
	citySprite->rect = rectd(-800, 0, 1600, 55);
	citySprite->textureMode = Sprite::kRepeatTextureMode;
	citySprite->autoTexRectX = true;
	
	//Initialize the crane sprite
	craneSprite = new Sprite;
	craneSprite->texture = Texture::named("simtower/068.bmp");
	craneSprite->rect.size = double2(36, 36);
}

void Tower::renderBackground(rectd visibleRect)
{
	//Calculate the rectangles
	rectd groundRect = visibleRect;
	groundRect.size.y = -groundRect.origin.y;
	rectd skyRect = visibleRect;
	skyRect.size.y += skyRect.origin.y;
	skyRect.origin.y = 0;
	
	//Draw tge ground sprite
	groundSprite->draw(visibleRect);
	
	//Draw the sky sprites
	for (int i = 0; i < 10; i++)
		skySprites[i]->draw(visibleRect);
	
	//Draw the city sprite
	citySprite->draw(visibleRect);
	
	//Draw the crane sprite
	craneSprite->draw(visibleRect);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Items
//----------------------------------------------------------------------------------------------------

/*Item * Tower::getFacility(unsigned int itemID, bool createIfInexistent = false)
{
	if (createIfInexistent || (facilityItems.count(itemID
}

Item * Tower::getTransport(unsigned int itemID, bool createIfInexistent = false)
{
}*/

unsigned int Tower::nextItemID()
{
	unsigned int i;
	for (i = 2; i < UINT_MAX; i++)
		if (facilityItems.count(i) == 0 && transportItems.count(i) == 0)
			return i;
	return UINT_MAX;
}

void Tower::setConstructionsHalted(bool halted)
{
	constructionsHalted = halted;
}

bool Tower::getConstructionsHalted()
{
	return constructionsHalted;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Cells
//----------------------------------------------------------------------------------------------------

Tower::Cell * Tower::getCell(int2 coordinates, bool createIfInexistent)
{
	if (createIfInexistent || (cells.count(coordinates.x) > 0 && cells[coordinates.x].count(coordinates.y) > 0))
		return &cells[coordinates.x][coordinates.y];
	return NULL;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

bool Tower::constructFlexibleWidthItem(Item::Descriptor * descriptor, recti currentRect, recti previousRect)
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
		Cell * cell = getCell(pos);
		Cell * cellBelow = getCell(posBelow);
		
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
				Item * facility = facilityItems[cell->facility];
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
	unsigned int itemID = nextItemID();
	
	//Add adjacent facilities of the same type to the list of hit facilities, so they get collapsed
	Cell * cellLeft = getCell(int2(validRect.minX() - 1, validRect.minY()));
	Cell * cellRight = getCell(int2(validRect.maxX(), validRect.minY()));
	if (cellLeft && cellLeft->facility > 1) {
		Item * facilityLeft = facilityItems[cellLeft->facility];
		if (facilityLeft->descriptor->type == descriptor->type) {
			OSSObjectLog << "adding adjacent facility " << facilityLeft->itemID << " on the left" << std::endl;
			facilitiesHit.insert(facilityLeft);
		}
	}
	if (cellRight && cellRight->facility > 1) {
		Item * facilityRight = facilityItems[cellRight->facility];
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
		facilityItems.erase((*it)->itemID);
	}
	OSSObjectLog << "the item rect is " << itemRect.description() << std::endl;
	
	//Make the cells the item covers point at it
	for (int x = itemRect.minX(); x < itemRect.maxX(); x++) {
		Cell * cell = getCell(int2(x, itemRect.origin.y), true);
		cell->facility = itemID;
	}
	
	//Create the new item and add it to the tower's facilities
	Item * item = Item::createNew(descriptor, itemRect, itemID);
	item->setTower(this);
	item->setUnderConstruction(true);
	facilityItems[itemID] = item;
	
	//If the item expands the tower's bounds vertically, reposition the crane
	if (itemRect.minY() >= bounds.maxY()) {
		int2 origin = itemRect.origin;
		origin.y += 1;
		craneSprite->rect.origin = convertCellToWorldCoordinates(origin) - double2(6, 0);
	}
	
	//Make the bounds cover the newly built item too
	bounds.unify(itemRect);
	
	return true;
}
