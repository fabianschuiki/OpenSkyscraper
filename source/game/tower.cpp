#include "tower.h"
#include "../classes.h"

#include "../resources/sound.h"

using namespace OSS;


//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

Tower::Tower()
{
	//Setup some basic tower layout attributes
	ceilingHeight = 12;
	cellSize = int2(8, 24 + ceilingHeight);
	
	//Initialize various components
	initBackground();
	initEnvironment();
	initConstruction();
	
	//Initialize the timer
	previousTime = time;
}

void Tower::initBackground()
{
	//Initialize the ground sprite
	groundSprite = new Sprite;
	groundSprite->texture = Texture::named("simtower/background/ground");
	groundSprite->rect = rectd(-800, -360, 1600, 360);
	groundSprite->textureMode = Sprite::kRepeatTextureMode;
	groundSprite->autoTexRectX = true;
	groundSprite->autoTexRectY = true;
	
	//Initialize the sky sprites
	for (int i = 0; i < 10; i++) {	
		char n[32];
		sprintf(n, "simtower/background/sky/%i/test", i);
		skySprites[i] = new Sprite;
		skySprites[i]->texture = Texture::named(n);
		skySprites[i]->rect = rectd(-800, i * 360, 1600, 360);
		skySprites[i]->textureMode = Sprite::kRepeatTextureMode;
		skySprites[i]->autoTexRectX = true;
	}
	
	//Initialize the city sprite
	citySprite = new Sprite;
	citySprite->texture = Texture::named("simtower/background/city");
	citySprite->rect = rectd(-800, 0, 1600, 55);
	citySprite->textureMode = Sprite::kRepeatTextureMode;
	citySprite->autoTexRectX = true;
	
	//Initialize the crane sprite
	craneSprite = new Sprite;
	craneSprite->texture = Texture::named("simtower/decoration/crane");
	craneSprite->rect.size = double2(36, 36);
}

void Tower::initEnvironment()
{
	//Initialize the basic environment variables
	time = 5.0;
	date = 0;
	rating = 1;
	funds = 2e6;
	population = 0;
	
	//Initialize the funds transfer sound effect
	fundsTransferSound.sound = Sound::named("simtower/cash");
	fundsTransferSound.layer = SoundEffect::kTopLayer;
	fundsTransferSound.minIntervalBetweenPlaybacks = 0.1;
	fundsTransferSound.loopCount = 2;
	fundsTransferSound.copyBeforeUse = true;
}

void Tower::initConstruction()
{
	//Initialize the basic construction variables
	constructionsHalted = false;
	
	//Initialize the flexible construction sound effect
	constructionSoundFlexible.sound = Sound::named("simtower/construction/flexible");
	constructionSoundFlexible.layer = SoundEffect::kTopLayer;
	constructionSoundFlexible.minIntervalBetweenPlaybacks = 0.4;
	constructionSoundFlexible.copyBeforeUse = true;
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
	//Advance the environment
	advanceTime(dt);
	
	//Advance the tower itself
	advanceFacilities(dt);
	advanceTransport(dt);
	
	//Advance the background
	advanceBackground(dt);
}

void Tower::advanceTime(double dt)
{
	//Decide at what speed the game time should be running
	double timeSpeed = 0.5;
	if (time > 1.5 && time < 6.0)
		timeSpeed = 1;
	if (time > 12 && time < 13)
		timeSpeed = 1.0 / 30;
	
	//Advance the game time
	previousTime = time;
	time += dt * timeSpeed;
	if (time > 24) {
		time -= 24;
		date++;
	}
}

void Tower::advanceFacilities(double dt)
{
	//Advance the facilities
	for (ItemMap::iterator it = facilityItems.begin(); it != facilityItems.end(); it++) {
		Item * item = it->second;
		if (item)
			item->advance(dt);
	}
}

void Tower::advanceTransport(double dt)
{
	//Advance the Transports
	for (ItemMap::iterator it = transportItems.begin(); it != transportItems.end(); it++) {
		Item * item = it->second;
		if (item)
			item->advance(dt);
	}
}

void Tower::advanceBackground(double dt)
{
	//Carking cock in the morning
	if (checkTime(5.0))
		Engine::shared()->audioTask.playSound(Sound::named("simtower/background/cock"),
											  SoundEffect::kTopLayer);
	
	//Birds
	if (checkTime(7.0))
		Engine::shared()->audioTask.playSound(Sound::named("simtower/background/birds/morning"),
											  SoundEffect::kTopLayer);
	if (checkTime(18.0))
		Engine::shared()->audioTask.playSound(Sound::named("simtower/background/birds/evening"),
											  SoundEffect::kTopLayer);
	
	//Bells at 10 o'clock
	if (checkTime(10.0))
		Engine::shared()->audioTask.playSound(Sound::named("simtower/background/bells"),
											  SoundEffect::kTopLayer);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Rendering
//----------------------------------------------------------------------------------------------------

void Tower::renderBackground(rectd visibleRect)
{
	//Calculate the rectangles
	rectd groundRect = visibleRect;
	groundRect.size.y = -groundRect.origin.y;
	rectd skyRect = visibleRect;
	skyRect.size.y += skyRect.origin.y;
	skyRect.origin.y = 0;
	
	//Draw tge ground sprite
	if (groundSprite)
		groundSprite->draw(visibleRect);
	
	//Draw the sky sprites
	for (int i = 0; i < 10; i++)
		if (skySprites[i])
			skySprites[i]->draw(visibleRect);
	
	//Draw the city sprite
	if (citySprite)
		citySprite->draw(visibleRect);
	
	//Draw the crane sprite
	if (craneSprite)
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
		if (craneSprite)
			craneSprite->rect.origin = convertCellToWorldCoordinates(origin) - double2(6, 0);
	}
	
	//Make the bounds cover the newly built item too
	bounds.unify(itemRect);
	
	//Play the construction sound
	Engine::shared()->audioTask.addSoundEffect(&constructionSoundFlexible);
	
	return true;
}

bool Tower::constructItem(Item::Descriptor * descriptor, recti rect)
{
	//Check whether the rect meets the descriptor's requirements
	if (!checkIfRectMeetsDescriptorRequirements(descriptor, rect))
		return false;
	
	//Perform a cell analysis
	int numEmptyCells, numOccupiedCells, numOccupiedCellsBelow, numOccupiedCellsAbove;
	analyzeCellsInRect(rect, &numEmptyCells, NULL, &numOccupiedCells,
					   &numOccupiedCellsBelow, &numOccupiedCellsAbove);
	
	//If the item is being built above ground the number of occupied cells below must match the
	//rect's width.
	if (rect.minY() >= 0 && numOccupiedCellsBelow != rect.size.x) {
		OSSObjectError << "cells below not valid" << std::endl;
		return false;
	}
	
	//If the item is being built below ground, vice versa.
	if (rect.maxY() <= 0 && numOccupiedCellsAbove != rect.size.x) {
		OSSObjectError << "cells above not valid" << std::endl;
		return false;
	}
	
	//There must not be any occupied cells inside the rect
	if (numOccupiedCells) {
		OSSObjectError << "occupied by other facilities" << std::endl;
		return false;
	}
	
	//Now the number of floor cells that have to be paid is known
	OSSObjectLog << "construction of " << numEmptyCells << " floor cells required" << std::endl;
	
	//Insert the new item
	insertNewItem(descriptor, rect);
	
	//Withdraw funds
	long costs = descriptor->price;
	costs += numEmptyCells * Item::descriptorForItemType(Item::kFloorType)->price;
	transferFunds(-costs);
	
	//Play the construction sound
	Engine::shared()->audioTask.playSound(Sound::named("simtower/construction/normal"),
										  SoundEffect::kTopLayer);
	
	return true;
}

bool Tower::checkIfRectMeetsDescriptorRequirements(Item::Descriptor * descriptor, recti rect)
{
	if (!(descriptor->attributes & Item::kAllowedOnGroundAttribute) &&
		rect.maxY() >= 1 && rect.minY() <= 0) {
		OSSObjectError << "not allowed on ground level" << std::endl;
		return false;
	}
	if ((descriptor->attributes & Item::kNotAboveGroundAttribute) && rect.maxY() > 0) {
		OSSObjectError << "above ground not allowed" << std::endl;
		return false;
	}
	if ((descriptor->attributes & Item::kNotBelowGroundAttribute) && rect.minY() < 0) {
		OSSObjectError << "below ground not allowed" << std::endl;
		return false;
	}
	if ((descriptor->attributes & Item::kEvery15thFloorAttribute) && (rect.origin.y % 15) != 0) {
		OSSObjectError << "only every 15th floor allowed" << std::endl;
		return false;
	}
	return true;
}

/**
 * Counts various cell types inside a rect. Handy if you have to decide whether a given rect is
 * valid for construction.
 */
void Tower::analyzeCellsInRect(recti rect,
							   int * numEmptyCells, int * numFloorCells, int * numOccupiedCells,
							   int * numOccupiedCellsBelow, int * numOccupiedCellsAbove)
{
	int emptyCells = 0, floorCells = 0, occupiedCells = 0;
	int occupiedCellsBelow = 0, occupiedCellsAbove = 0;
	
	//Iterate through the item rect horizontally
	for (int x = rect.minX(); x < rect.maxX(); x++) {
		
		//Count occupied cells below
		Cell * cellBelow = getCell(int2(x, rect.minY() - 1), false);
		if (cellBelow && cellBelow->facility)
			occupiedCellsBelow++;
		
		//Count occupied cells above
		Cell * cellAbove = getCell(int2(x, rect.maxY()), false);
		if (cellAbove && cellAbove->facility)
			occupiedCellsAbove++;
		
		//Count cells inside
		for (int y = rect.minY(); y < rect.maxY(); y++) {
			Cell * cell = getCell(int2(x, y), false);
			if (!cell || !cell->facility)
				emptyCells++;
			else if (facilityItems[cell->facility]->descriptor->type == Item::kFloorType)
				floorCells++;
			else
				occupiedCells++;
		}
	}
	
	//Return the result
	if (numEmptyCells) *numEmptyCells = emptyCells;
	if (numFloorCells) *numFloorCells = floorCells;
	if (numOccupiedCells) *numOccupiedCells = occupiedCells;
	if (numOccupiedCellsBelow) *numOccupiedCellsBelow = occupiedCellsBelow;
	if (numOccupiedCellsAbove) *numOccupiedCellsAbove = occupiedCellsAbove;
}

/**
 * Inserts a new item into the tower at the given location. Only the insertion is done, none of the
 * construction-related stuff like validation, funds transfer, etc. is accomplished. Use the
 * construct* functions instead.
 */
void Tower::insertNewItem(Item::Descriptor * descriptor, recti rect)
{
	//Fetch a new item ID
	unsigned int itemID = nextItemID();
	
	//Make the cells the item covers point at it
	for (int x = rect.minX(); x < rect.maxX(); x++) {
		for (int y = rect.minY(); y < rect.maxY(); y++) {
			Cell * cell = getCell(int2(x, y), true);
			cell->facility = itemID;
		}
	}
	
	//Create the new item and add it to the tower's facilities
	Item * item = Item::createNew(descriptor, rect, itemID);
	item->setTower(this);
	item->setUnderConstruction(true);
	facilityItems[itemID] = item;
	
	//If the item expands the tower's bounds vertically, reposition the crane
	if (rect.minY() >= bounds.maxY()) {
		int2 origin = rect.origin;
		origin.y += 1;
		if (craneSprite)
			craneSprite->rect.origin = convertCellToWorldCoordinates(origin) - double2(6, 0);
	}
	
	//Make the bounds cover the newly built item too
	bounds.unify(rect);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Environment
//----------------------------------------------------------------------------------------------------

unsigned int Tower::getDayOfWeek()
{
	return (date % 3);
}

unsigned int Tower::getQuarter()
{
	return ((date / 3) % 4);
}

unsigned int Tower::getYear()
{
	return (date / 12);
}

bool Tower::isWeekday()
{
	return (getDayOfWeek() < 2);
}

bool Tower::isWeekend()
{
	return (getDayOfWeek() == 2);
}

void Tower::transferFunds(long amount)
{
	funds += amount;
	Engine::shared()->audioTask.addSoundEffect(&fundsTransferSound);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Timer
//----------------------------------------------------------------------------------------------------

bool Tower::checkTime(double previousTime, double alarmTime)
{
	return (alarmTime > previousTime && alarmTime <= time);
}

bool Tower::checkTime(double alarmTime)
{
	return checkTime(previousTime, alarmTime);
}
