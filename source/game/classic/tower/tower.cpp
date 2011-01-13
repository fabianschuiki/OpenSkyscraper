#include "tower.h"

#include "../items/item.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

Tower::Tower() : Classic::Responder(this)
{	
	//Initialize the subsystems
	time = new TowerTime(this);
	environment = new TowerEnvironment(this);
	funds = new TowerFunds(this);
	background = new TowerBackground(this);
	structure = new TowerStructure(this);
	
	//DEBUG: setup the debug speed
	debugSpeed = 0;
	paused = false;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Dimensions
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


recti Tower::getGroundFloorRect() const
{
	recti r = bounds;
	r.size.y = 1;
	r.origin.y = 0;
	return r;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Notifications
//----------------------------------------------------------------------------------------------------

void Tower::onChangeTransportItems()
{
	for (ItemIDMap::iterator pair = facilityItems.begin(); pair != facilityItems.end(); pair++)
		if (pair->second)
			pair->second->onChangeTransportItems();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

void Tower::advance(double dt)
{
	if (paused) return;
	
	//Advance the subsystems
	time->advance(dt);
	environment->advance(dt);
	funds->advance(dt);
	background->advance(dt);
	structure->advance(dt);
	
	//If any of the subsystems needs updating, we also do
	if (time->updateIfNeeded.isNeeded() ||
		environment->updateIfNeeded.isNeeded() ||
		funds->updateIfNeeded.isNeeded() ||
		background->updateIfNeeded.isNeeded() ||
		structure->updateIfNeeded.isNeeded())
		updateIfNeeded.setNeeded();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark State
//----------------------------------------------------------------------------------------------------

void Tower::update()
{
	//Update the subsystems
	time->updateIfNeeded();
	environment->updateIfNeeded();
	funds->updateIfNeeded();
	background->updateIfNeeded();
	structure->updateIfNeeded();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

void Tower::draw(rectd dirtyRect)
{
	//Draw the background
	background->draw(dirtyRect);
	
	//Draw the tower structure
	structure->draw(dirtyRect);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Events
//----------------------------------------------------------------------------------------------------

bool Tower::sendEventToNextResponders(OSS::Event * event)
{
	if (time && time->sendEvent(event)) return true;
	if (environment && environment->sendEvent(event)) return true;
	if (funds && funds->sendEvent(event)) return true;
	if (structure && structure->sendEvent(event)) return true;
	if (background && background->sendEvent(event)) return true;
	return BasicResponder::sendEventToNextResponders(event);
}

/*void Tower::advanceBackground(double dt)
{
	//Decide whether this is going to be a rainy day
	if (checkTime(5.0))
		isRainyDay = (randui(0, 5) == 0);
	
	//Carking cock in the morning
	if (checkTime(5.5))
		Audio::shared()->play(Sound::named("simtower/background/cock"),
										  SoundEffect::kTopLayer);
	
	//Birds
	if (checkTime(6.0))
		Audio::shared()->play(&birdsMorningSound);
	if (checkTime(18.0))
		Audio::shared()->play(Sound::named("simtower/background/birds/evening"),
										  SoundEffect::kTopLayer);
	
	//Bells at 10 o'clock
	if (checkTime(10.0))
		Audio::shared()->play(Sound::named("simtower/background/bells"),
										  SoundEffect::kTopLayer);
	
	//Sky State
	if (time < 5 || time >= 19)
		setSkyState(kNightState);
	else if (time >= 5 && time < 6)
		setSkyState(kNightState, kTwilightState, (time - 5));
	else if (time >= 6 && time < 7)
		setSkyState(kTwilightState, kDayState, (time - 6));
	else if (time >= 17 && time < 18)
		setSkyState(kDayState, kTwilightState, (time - 17));
	else if (time >= 18 && time < 19)
		setSkyState(kTwilightState, kNightState, (time - 18));
	else if (!isRainyDay)
		setSkyState(kDayState);
	else if (time >= 7 && time < 8)
		setSkyState(kDayState, kOvercastState, (time - 7));
	else if (time >= 16 && time < 17)
		setSkyState(kOvercastState, kDayState, (time - 16));
	else {
		rainAnimationTime += 2 * dt;
		while (rainAnimationTime >= 1)
			rainAnimationTime--;
		setSkyState(kRainState);
	}
	
	//Play the additional sounds for rainy days
	if (isRainyDay) {
		//Start and stop the rain sound effect
		if (checkTime(8))
			Audio::shared()->play(&rainSound);
		if (checkTime(16))
			rainSound.stop();
		
		//Set the thunder countdown to 0 if it just started raining to introduce the weather ;)
		if (checkTime(8))
			nextThunderCountdown = 0.0;
		
		//Decrease the thunder countdown and play the sound effect if appropriate
		nextThunderCountdown -= dt;
		if (time >= 8 && time < 16 && nextThunderCountdown <= 0) {
			Audio::shared()->play(Sound::named("simtower/background/thunder"),
											  SoundEffect::kTopLayer);
			nextThunderCountdown = randd(3, 15.0);
			OSSObjectLog << "next thunder in " << nextThunderCountdown << "s" << std::endl;
		}
	}
}*/





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Background
//----------------------------------------------------------------------------------------------------

/*void Tower::renderBackground(rectd visibleRect)
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
		for (int n = 0; n < 2; n++)
			if (skySprites[i][n])
				skySprites[i][n]->draw(visibleRect);
	
	//Draw the city sprite
	if (citySprite)
		citySprite->draw(visibleRect);
	
	//Draw the crane sprite
	if (craneSprite)
		craneSprite->draw(visibleRect);
}

void Tower::setSkyState(SkyState state)
{
	setSkyState(state, skyState[1], 0.0);
}

void Tower::setSkyState(SkyState current, SkyState target, double interpolation)
{
	//Recalculate the rainIndex
	unsigned int newRainIndex = (rainAnimationTime * 2);
	bool rainIndexChanged = (newRainIndex != rainIndex);
	rainIndex = newRainIndex;
	
	//Store the states and update the sky textures
	if (skyState[0] != current || (rainIndexChanged && skyState[0] == kRainState)) {
		skyState[0] = current;
		updateSkySpriteTextures(0);
	}
	if (skyState[1] != target || (rainIndexChanged && skyState[1] == kRainState)) {
		skyState[1] = target;
		updateSkySpriteTextures(1);
	}
	
	//Update the sky sprite alpha values
	for (int i = 0; i < 10; i++) {
		skySprites[i][0]->color.c.a = 1.0;
		skySprites[i][1]->color.c.a = interpolation;
	}
}

void Tower::updateSkySpriteTextures(unsigned int stateIndex)
{
	for (int i = 0; i < 10; i++) {
		//Load the base texture name
		string textureName("simtower/background/sky/");
		
		//Add the vertical index
		char str[16];
		sprintf(str, "%i/", i);
		textureName += str;
		
		//Add the sky state
		switch (skyState[stateIndex]) {
			case kDayState: textureName += "day"; break;
			case kTwilightState: textureName += "twilight"; break;
			case kNightState: textureName += "night"; break;
			case kOvercastState: textureName += "overcast"; break;
			case kRainState: {
				sprintf(str, "rain/%i", rainIndex);
				textureName += str;
			} break;
		}
		
		//Load the appropriate texture
		skySprites[i][stateIndex]->texture = Texture::named(textureName);
	}
}*/





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

Item * Tower::getGroundLobby()
{
	ItemSet * floor0Facilities = &facilityItemsByFloor[0];
	ItemSet::iterator item = floor0Facilities->begin();
	if (item == floor0Facilities->end())
		return NULL;
	return *item;
}

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

/*bool Tower::constructFlexibleWidthItem(ItemDescriptor * descriptor, recti currentRect, recti previousRect)
{
	OSSObjectLog << "attempting construction of " << descriptor->type << " from " << previousRect.description() << " to " << currentRect.description() << std::endl;
	
	//Check whether the basic construction limitations are given
	if ((descriptor->attributes & kEvery15thFloorAttribute) && (currentRect.origin.y % 15) != 0) {
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
	double cost = floorCellsRequired * Item::descriptorForItemType(kFloorType)->price;
	cost += itemCellsRequired * descriptor->price;
	
	//Check whether the money is available...
	OSSObjectLog << "construction costs " << cost << std::endl;
	
	//Calculate the valid rect
	recti validRect(std::min<int>(scanOrigin.x, lastValidPosition.x), scanOrigin.y, abs(scanOrigin.x - lastValidPosition.x) + 1, 1);
	OSSObjectLog << "the item's valid rect is " << validRect.description() << std::endl;
	
	//Fetch the item ID we will assign the new item
	//unsigned int itemID = nextItemID();
	
	//Add adjacent facilities of the same type to the list of hit facilities, so they get collapsed
	Cell * cellLeft = getCell(int2(validRect.minX() - 1, validRect.minY()));
	Cell * cellRight = getCell(int2(validRect.maxX(), validRect.minY()));
	if (cellLeft && cellLeft->facility > 1) {
		Item * facilityLeft = facilityItems[cellLeft->facility];
		if (facilityLeft->descriptor->type == descriptor->type)
			facilitiesHit.insert(facilityLeft);
	}
	if (cellRight && cellRight->facility > 1) {
		Item * facilityRight = facilityItems[cellRight->facility];
		if (facilityRight->descriptor->type == descriptor->type)
			facilitiesHit.insert(facilityRight);
	}
	
	//Calculate the item rect which actually is a union of the validRect and all the hit facilities.
	//We also need to get rid of the hit facilities themselves
	recti itemRect = validRect;
	std::set<Item *>::iterator it;
	for (it = facilitiesHit.begin(); it != facilitiesHit.end(); it++) {
		itemRect.unify((*it)->getRect());
		//facilityItems.erase((*it)->getItemID());
		eraseItem(*it);
	}
	OSSObjectLog << "the item rect is " << itemRect.description() << std::endl;
	
	//Insert the new item
	insertNewItem(descriptor, itemRect);
	
	//Play the construction sound
	Audio::shared()->play(&constructionSoundFlexible);
	
	return true;
}

bool Tower::constructItem(ItemDescriptor * descriptor, recti rect)
{
	//Check whether the rect meets the descriptor's requirements
	if (!checkIfRectMeetsDescriptorRequirements(descriptor, rect))
		return false;
	
	//Perform a cell analysis
	CellAnalysis analysis = analyzeCellsInRect(rect, descriptor->mask);
	OSSObjectLog << "analysis returned " << analysis.unmaskedCells << " masked cells" << std::endl;
	
	//Perform category-specific checks
	switch (descriptor->category) {
			//Facility items
		case kFacilityCategory: {
			//If the item is being built above ground the number of occupied cells below must match the
			//rect's width.
			if (rect.minY() >= 0 && analysis.facilityCellsBelow != rect.size.x) {
				OSSObjectError << "cells below not valid" << std::endl;
				return false;
			}
			
			//If the item is being built below ground, vice versa.
			if (rect.maxY() <= 0 && analysis.facilityCellsAbove != rect.size.x) {
				OSSObjectError << "cells above not valid" << std::endl;
				return false;
			}
			
			//There must not be any occupied cells inside the rect
			if (analysis.masked.facility) {
				OSSObjectError << "occupied by other facilities" << std::endl;
				return false;
			}
		} break;
			
			//Transport items
		case kTransportCategory: {
			//The transport item must be entirely covered by facilities
			if (analysis.all.facility != rect.area()) {
				OSSObjectError << "not entirely occupied by facilities" << std::endl;
				return false;
			}
			
			//There must not be any cells occupied by transports inside the rect
			if (analysis.masked.transport) {
				OSSObjectError << "occupied by other transports" << std::endl;
				return false;
			}
		} break;
	}
	
	//Insert the new item
	insertNewItem(descriptor, rect);
	
	//Withdraw funds
	long costs = descriptor->price;
	costs += analysis.masked.empty * Item::descriptorForItemType(kFloorType)->price;
	funds->transfer(-costs);
	
	//Play the construction sound
	Audio::shared()->play(Sound::named("simtower/construction/normal"),
									  SoundEffect::kTopLayer);
	
	return true;
}*/

bool Tower::checkIfRectMeetsDescriptorRequirements(ItemDescriptor * descriptor, recti rect)
{
	switch (descriptor->category) {
			//Facility limitations
		case kFacilityCategory: {
			if (!(descriptor->attributes & kAllowedOnGroundAttribute) &&
				rect.maxY() >= 1 && rect.minY() <= 0) {
				OSSObjectError << "not allowed on ground level" << std::endl;
				return false;
			}
			if ((descriptor->attributes & kNotAboveGroundAttribute) && rect.maxY() > 0) {
				OSSObjectError << "above ground not allowed" << std::endl;
				return false;
			}
			if ((descriptor->attributes & kNotBelowGroundAttribute) && rect.minY() < 0) {
				OSSObjectError << "below ground not allowed" << std::endl;
				return false;
			}
			if ((descriptor->attributes & kEvery15thFloorAttribute) && (rect.origin.y % 15) != 0) {
				OSSObjectError << "only every 15th floor allowed" << std::endl;
				return false;
			}
		} break;
			
			//Transport limitations
		case kTransportCategory: {
		} break;
	}
	return true;
}

/**
 * Counts various cell types inside a rect. Handy if you have to decide whether a given rect is
 * valid for construction.
 */
Tower::CellAnalysis Tower::analyzeCellsInRect(recti rect, rectmaski mask)
{
	CellAnalysis analysis;
	bzero(&analysis, sizeof(analysis));
	
	//Iterate through the item rect horizontally
	for (int x = rect.minX(); x < rect.maxX(); x++) {
		
		//Count occupied cells below
		Cell * cellBelow = getCell(int2(x, rect.minY() - 1), false);
		if (cellBelow && cellBelow->facility)
			analysis.facilityCellsBelow++;
		
		//Count occupied cells above
		Cell * cellAbove = getCell(int2(x, rect.maxY()), false);
		if (cellAbove && cellAbove->facility)
			analysis.facilityCellsAbove++;
		
		//Count cells inside
		for (int y = rect.minY(); y < rect.maxY(); y++) {
			int2 point(x, y);
			
			//Check whether this cell is masked
			bool masked = (mask.containsPoint(point - rect.origin));
			
			//Count masked cells
			if (!masked)
				analysis.unmaskedCells++;
			
			//Fetch the cell instance
			Cell * cell = getCell(point, false);
			
			//Count the facility cells
			if (!cell || !cell->facility) {
				analysis.all.empty++;
				if (masked) analysis.masked.empty++;
			}
			else if (facilityItems[cell->facility]->descriptor->type == kFloorType) {
				analysis.all.floor++;
				if (masked) analysis.masked.floor++;
			}
			else {
				analysis.all.facility++;
				if (masked) analysis.masked.facility++;
			}
			
			//Count transport cells
			if (cell && cell->transport) {
				analysis.all.transport++;
				if (masked) {
					OSSObjectLog << "cell at " << point.description() << " occupied by transport!" << std::endl;
					analysis.masked.transport++;
				}
			}
		}
	}
	
	//Return the result
	return analysis;
}

/**
 * Inserts a new item into the tower at the given location. Only the insertion is done, none of the
 * construction-related stuff like validation, funds transfer, etc. is accomplished. Use the
 * construct* functions instead.
 */
/*void Tower::insertNewItem(ItemDescriptor * descriptor, recti rect)
{
	//Fetch a new item ID
	unsigned int itemID = nextItemID();
	
	//DEBUG: Log the opacity
	OSSObjectLog << descriptor->mask.description() << std::endl;
	
	//Make the cells the item covers point at it
	for (int x = rect.minX(); x < rect.maxX(); x++) {
		for (int y = rect.minY(); y < rect.maxY(); y++) {
			int2 point(x, y);
			if (!descriptor->mask.containsPoint(point - rect.origin)) continue;
			Cell * cell = getCell(point, true);
			switch (descriptor->category) {
				case kFacilityCategory:		cell->facility	= itemID; break;
				case kTransportCategory:	cell->transport	= itemID; break;
			}
		}
	}
	
	//Create the new item and add it to the tower's facilities
	Item * item = Item::make(this, descriptor, itemID, rect);
	ItemIDMap * idMap = NULL;
	ItemFloorMap * floorMap = NULL;
	switch (descriptor->category) {
		case kFacilityCategory: {
			idMap = &facilityItems;
			floorMap = &facilityItemsByFloor;
			item->setUnderConstruction(true);
		} break;
		case kTransportCategory: {
			idMap = &transportItems;
			floorMap = &transportItemsByFloor;
		} break;
	}
	
	//Add the item to the appropriate maps
	assert(idMap && floorMap);
	items[itemID] = item;
	(*idMap)[itemID] = item;
	for (int y = rect.minY(); y < rect.maxY(); y++) {
		itemsByFloor[y].insert(item);
		(*floorMap)[y].insert(item);
	}
	
	//If the item expands the tower's bounds vertically, reposition the crane
	if (rect.minY() >= bounds.maxY()) {
		int2 origin = rect.origin;
		origin.y += 1;
		if (craneSprite)
			craneSprite->rect.origin = convertCellToWorldCoordinates(origin) - double2(6, 0);
	}
	
	//Make the bounds cover the newly built item too
	bounds.unify(rect);
	
	//Post a notification
	if (descriptor->category == kTransportCategory)
		onChangeTransportItems();
}*/

void Tower::eraseItem(Item * item)
{
	eraseItem(item->getItemID(), item);
}

void Tower::eraseItem(unsigned int itemID)
{
	eraseItem(itemID, items[itemID]);
}

void Tower::eraseItem(unsigned int itemID, Item * item)
{
	item->retain();
	items.erase(itemID);
	facilityItems.erase(itemID);
	transportItems.erase(itemID);
	
	ItemFloorMap::iterator pair;
	for (pair = itemsByFloor.begin(); pair != itemsByFloor.end(); pair++)
		pair->second.erase(item);
	for (pair = facilityItemsByFloor.begin(); pair != facilityItemsByFloor.end(); pair++)
		pair->second.erase(item);
	for (pair = transportItemsByFloor.begin(); pair != transportItemsByFloor.end(); pair++)
		pair->second.erase(item);
	
	item->setItemID(0);
	item->release();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Environment
//----------------------------------------------------------------------------------------------------

bool Tower::didDateAdvance()
{
	return dateAdvanced;
}

/*unsigned int Tower::getLobbyStyle()
{
	if (rating >= 4)
		return 2;
	if (rating >= 3)
		return 1;
	return 0;
}

void Tower::transferFunds(long amount)
{
	funds += amount;
	Audio::shared()->play(&fundsTransferSound);
}*/





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Timer
//----------------------------------------------------------------------------------------------------

bool Tower::checkTime(double previousTime, double alarmTime)
{
	//return (alarmTime > previousTime && alarmTime <= time);
	return false;
}

bool Tower::checkTime(double alarmTime)
{
	//DEBUG: re-enable this!
	//OSSObjectError << "using the deprecated checkTime()!" << std::endl;
	return time->checkDaily(alarmTime);
}
