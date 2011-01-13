#include "structure.h"

#include "../items/item.h"
#include "tower.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

TowerStructure::TowerStructure(Tower * tower) : tower(tower),
ceilingHeight(12), cellSize(8, 24 + 12)
{
	//Initialize the construction sound effect
	constructionSound = new SoundEffect();
	constructionSound->sound = Sound::named("simtower/construction/normal");
	constructionSound->layer = SoundEffect::kTopLayer;
	constructionSound->copyBeforeUse = true;
	
	//Initialize the construction sound effect for flexible width items like lobbies. It sounds
	//different to build thos.
	flexibleConstructionSound = new SoundEffect();
	flexibleConstructionSound->sound = Sound::named("simtower/construction/flexible");
	flexibleConstructionSound->layer = SoundEffect::kTopLayer;
	flexibleConstructionSound->minIntervalBetweenPlaybacks = 0.4;
	flexibleConstructionSound->copyBeforeUse = true;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Dimensions
//----------------------------------------------------------------------------------------------------

double2 TowerStructure::cellToWorld(int2 v)
{
	return (v * cellSize);
}

rectd TowerStructure::cellToWorld(recti v)
{
	return rectd(v.origin * cellSize, v.size * cellSize);
}



int2 TowerStructure::worldToCell(double2 v)
{
	return int2(round(v.x / cellSize.x), round(v.y / cellSize.y));
}

recti TowerStructure::worldToCell(rectd v)
{
	recti rect;
	rect.origin = worldToCell(v.origin);
	rect.size = worldToCell(v.origin + v.size) - rect.origin;
	return rect;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Cells
//----------------------------------------------------------------------------------------------------

TowerStructure::Cell * TowerStructure::getCell(int2 location, bool createIfInexistent)
{
	Cell * cell = NULL;
	bool exists = (cells.count(location.y) && cells[location.y].count(location.x));
	if (createIfInexistent || exists)
		cell = &cells[location.y][location.x];
	if (cell && !exists)
		cell->location = location;
	return cell;
}



TowerStructure::CellSet TowerStructure::getCells(recti rect, bool createIfInexistent)
{
	CellSet cells;
	for (int y = rect.minY(); y < rect.maxY(); y++) {
		for (int x = rect.minX(); x < rect.maxX(); x++) {
			Cell * cell = getCell(int2(x, y), createIfInexistent);
			if (cell)
				cells.insert(cell);
		}
	}
	return cells;
}

TowerStructure::CellSet TowerStructure::getCells(rectmaski rectmask, bool createIfInexistent)
{
	CellSet cells = getCells(rectmask.bounds(), createIfInexistent);
	for (CellSet::iterator it = cells.begin(); it != cells.end(); it++)
		if (!rectmask.containsPoint((*it)->location))
			cells.erase(it);
	return cells;
}



TowerStructure::CellAnalysis TowerStructure::analyseCells(CellSet cells)
{
	CellAnalysis analysis;
	bzero(&analysis, sizeof(analysis));
	
	//Iterate through the cells
	for (CellSet::iterator it = cells.begin(); it != cells.end(); it++) {
		Cell * cell = *it;
		assert(cell);
		
		//Count the facility items and floors
		if (cell->items[kFacilityCategory])
			analysis.facility++;
		else
			analysis.floor++;
		
		//Count the transports
		if (cell->items[kTransportCategory]) analysis.transport++;
	}
	
	return analysis;
}

TowerStructure::CellAnalysis TowerStructure::analyseCells(recti rect)
{
	return analyseCells(getCells(rect));
}

TowerStructure::CellAnalysis TowerStructure::analyseCells(rectmaski rectmask)
{
	return analyseCells(getCells(rectmask));
}



void TowerStructure::assignCellsCoveredByItem(Item * item)
{
	if (!item) return;
	
	//Get the cells the item covers
	CellSet cells = getCells(item->getOccupiedRectMask(), true);
	
	//Set the item in each cell's item slot
	for (CellSet::iterator it = cells.begin(); it != cells.end(); it++) {
		Item ** slot = &(*it)->items[item->getCategory()];
		assert(*slot == NULL && "occupied by another item. check before adding.");
		*slot = item;
	}
}

void TowerStructure::unassignCellsCoveredByItem(Item * item)
{
	if (!item) return;
	
	//Get the cells the item covers
	CellSet cells = getCells(item->getOccupiedRectMask(), false);
	
	//Unset the item in each cell's item slot
	for (CellSet::iterator it = cells.begin(); it != cells.end(); it++) {
		Item ** slot = &(*it)->items[item->getCategory()];
		assert(*slot == item && "not all cells covered by the item are assigned to it!");
		*slot = NULL;
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Items
//----------------------------------------------------------------------------------------------------

const TowerStructure::ItemPointerSet & TowerStructure::getItems()
{
	return items;
}

const TowerStructure::ItemSet & TowerStructure::getItems(ItemType type)
{
	return itemsByType[type];
}

const TowerStructure::ItemSet & TowerStructure::getItems(ItemGroup group)
{
	return itemsByGroup[group];
}

const TowerStructure::ItemSet & TowerStructure::getItems(ItemCategory category)
{
	return itemsByCategory[category];
}



const TowerStructure::ItemSet & TowerStructure::getItems(int floor)
{
	return itemsByFloor[floor];
}

const TowerStructure::ItemSet & TowerStructure::getItems(int floor, ItemType type)
{
	return itemsByFloorAndType[floor][type];
}

const TowerStructure::ItemSet & TowerStructure::getItems(int floor, ItemGroup group)
{
	return itemsByFloorAndGroup[floor][group];
}

const TowerStructure::ItemSet & TowerStructure::getItems(int floor, ItemCategory category)
{
	return itemsByFloorAndCategory[floor][category];
}



TowerStructure::ItemSet TowerStructure::getItems(CellSet cells)
{
	ItemSet items;
	for (CellSet::iterator it = cells.begin(); it != cells.end(); it++)
		for (ItemByCategory::iterator ic = (*it)->items.begin(); ic != (*it)->items.end(); ic++)
			if (ic->second)
				items.insert(ic->second);
	return items;
}

TowerStructure::ItemSet TowerStructure::getItems(recti rect)
{
	return getItems(getCells(rect));
}

TowerStructure::ItemSet TowerStructure::getItems(rectmaski rectmask)
{
	return getItems(getCells(rectmask));
}



void TowerStructure::addItem(Item * item)
{
	if (!item) return;
	
	//Insert the item into the overall item container
	items.insert(item);
	
	//Insert into the type, group and category split ups
	itemsByType[item->getType()].insert(item);
	itemsByGroup[item->getGroup()].insert(item);
	itemsByCategory[item->getCategory()].insert(item);
	
	//Insert into the floor split ups
	for (int i = item->getMinFloor(); i <= item->getMaxFloor(); i++) {
		itemsByFloor[i].insert(item);
		itemsByFloorAndType[i][item->getType()].insert(item);
		itemsByFloorAndGroup[i][item->getGroup()].insert(item);
		itemsByFloorAndCategory[i][item->getCategory()].insert(item);
	}
	
	//Insert into the cell map
	assignCellsCoveredByItem(item);
}

void TowerStructure::removeItem(Item * item)
{
	if (!item) return;
	
	//Remove the item from the cell map
	unassignCellsCoveredByItem(item);
	
	//Remove the item from the type, group and category split ups
	itemsByType[item->getType()].erase(item);
	itemsByGroup[item->getGroup()].erase(item);
	itemsByCategory[item->getCategory()].erase(item);
	
	//Remove the item from the floor split ups
	for (int i = item->getMinFloor(); i <= item->getMaxFloor(); i++) {
		itemsByFloor[i].erase(item);
		itemsByFloorAndType[i][item->getType()].erase(item);
		itemsByFloorAndGroup[i][item->getGroup()].erase(item);
		itemsByFloorAndCategory[i][item->getCategory()].erase(item);
	}
	
	//Remove the item from the overall container. This will also release the reference we have on
	//the item.
	items.erase(item);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Reports
//----------------------------------------------------------------------------------------------------

TowerStructure::Report TowerStructure::getReport(recti rect, ItemDescriptor * descriptor)
{
	Report report;
	if (!descriptor)
		return report;
	
	//Check the attributes if they are fulfilled
	report.unfulfilledAttributes = 0;
	
	//Allowed on ground
	if (!(descriptor->attributes & kAllowedOnGroundAttribute) && rect.maxY() > 0 && rect.minY() <= 0)
		report.unfulfilledAttributes |= kAllowedOnGroundAttribute;
	
	//Above/below ground
	if ((descriptor->attributes & kNotAboveGroundAttribute) && rect.maxY() > 0)
		report.unfulfilledAttributes |= kNotAboveGroundAttribute;
	if ((descriptor->attributes & kNotBelowGroundAttribute) && rect.minY() < 0)
		report.unfulfilledAttributes |= kNotBelowGroundAttribute;
	
	//Every 15th floor
	if ((descriptor->attributes & kEvery15thFloorAttribute) && (rect.minY() % 15) != 0)
		report.unfulfilledAttributes |= kEvery15thFloorAttribute;
	
	
	//Check if the items above/below are valid
	recti rectAbove(rect.origin.x, rect.maxY(), rect.size.x, 1);
	recti rectBelow(rect.origin.x, rect.minY() - 1, rect.size.x, 1);
	report.cellsAboveValid = (analyseCells(rectAbove).facility == rectAbove.size.x);
	report.cellsBelowValid = (analyseCells(rectBelow).facility == rectBelow.size.x);
	
	//Decide whether the adjacent cells (above/below) are valid for the given rect
	if (rect.minY() > 0)
		report.adjacentCellsValid = report.cellsBelowValid;
	else if (rect.maxY() <= 0)
		report.adjacentCellsValid = report.cellsAboveValid;
	else
		report.adjacentCellsValid = true;
	
	
	//Calculate the local rectmask
	rectmaski localMask = descriptor->mask;
	if (localMask.empty())
		localMask = rectmaski(&rect, NULL);
	else
		localMask.offset(rect.origin);
	
	
	//Get all the items that are affected by the rectangle
	ItemSet items = getItems(rect);
	
	//Iterate through the items and check whether they collide in any form with the item
	for (ItemSet::iterator it = items.begin(); it != items.end(); it++)
		if ((*it)->getCategory() == descriptor->category)
			if (descriptor->mask.intersectsRect((*it)->getRect()))
				report.collidesWith.insert((Item *)(*it));
	
	
	//Analyze the cells to find how many additional cells will be required
	CellAnalysis analysis = analyseCells(localMask);
	report.additionalFacilityCellsRequired = (localMask.area() - analysis.facility);
	report.additionalFloorCellsRequired = (report.additionalFacilityCellsRequired - analysis.floor);
	
	
	//Facilities and transports have different requirements
	report.validForFacility = (!report.unfulfilledAttributes &&
							   report.adjacentCellsValid &&
							   (analysis.facility == 0 || descriptor->attributes & kFlexibleWidthAttribute));
	report.validForTransport = (!report.unfulfilledAttributes &&
								report.adjacentCellsValid &&
								analysis.transport == 0 &&
								report.additionalFacilityCellsRequired == 0);
	
	//Check the overall validity of the location based on the descriptor's category
	report.valid = false;
	switch (descriptor->category) {
		case kFacilityCategory:		report.valid = report.validForFacility; break;
		case kTransportCategory:	report.valid = report.validForTransport; break;
	}
	
	return report;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Item Construction
//----------------------------------------------------------------------------------------------------

TowerStructure::ConstructionResult TowerStructure::constructItem(ItemDescriptor * descriptor,
																 recti rect, recti initialRect)
{
	if (!descriptor)
		return (ConstructionResult){false, ""};
	
	//First of all we need to find the union rect between rect and initialRect
	recti buildRect = rect.unionRect(initialRect);
	
	//Decide whether this is a flexible width item
	bool isFlexible = descriptor->attributes & kFlexibleWidthAttribute;
	
	//In case we're building the lobby on the ground floor, we want that if there's already a lobby,
	//the one we're building collapses entirely with the other one, so that you could build a 4x1
	//lobby on the left and one on the right of the screen and the two would connect to one huge
	//lobby.
	if (descriptor->type == kLobbyType && rect.minY() == 0) {
		
		//Find all the lobbies on the ground floor and unify the construction rect with their rect.
		ItemSet lobbies = getItems(0, kLobbyType);
		for (ItemSet::iterator it = lobbies.begin(); it != lobbies.end(); it++)
			buildRect.unify((*it)->getRect());
	}
	
	
	//To star constructing stuff we need a report on the rect that we're trying to build in so we
	//can decide whether it is suitable for construction and how much it will cost.
	Report report = getReport(buildRect, descriptor);
	
	
	//Find the actualy rect we will build in. Flexible items need some special treatment.
	recti actualRect = buildRect;
	if (isFlexible) {
		
		//We have to find the acutal rect the final item will have. This means iterating through all
		//items that we collided with and finding the one closest to the initial rect. That's where the
		//actual item rect will end.
		for (ItemSet::iterator it = report.collidesWith.begin(); it != report.collidesWith.end(); it++) {
			
			//Skip items that are of the same type as we are, since we will collapse with them in the
			//end anyway. Also skip items that are NOT of the same category as we are.
			if ((*it)->getType() == descriptor->type || (*it)->getCategory() != descriptor->category)
				continue;
			
			//Skip the collision item if its rect doesn't collide anymore since we were already forced
			//to reduce the actual rect.
			if (!actualRect.intersectsRect((*it)->getRect()))
				continue;
			
			
			//Calculate the rect containing the initial rect and touching the item's rect. If it's
			//smaller than the current actual rect it has to take its place since we hit a facility
			//somewhere.
			recti itemRect = (*it)->getRect();
			recti reducedRect = actualRect;
			
			//If the item is further to the left than the initial rect we have to do a cutoff on the
			//left.
			if (itemRect.maxX() <= initialRect.minX()) {
				int leftCutOff = itemRect.maxX() - reducedRect.minX();
				reducedRect.origin.x += leftCutOff;
				reducedRect.size.x -= leftCutOff;
			}
			
			//And likewise on the right.
			if (itemRect.minX() > initialRect.maxX()) {
				int rightCutOff = reducedRect.maxX() - itemRect.minX();
				reducedRect.size.x -= rightCutOff;
			}
			
			//If the reduced rect is smaller than the actual rect, it needs to take its place.
			if (reducedRect.area() < actualRect.area())
				actualRect = reducedRect;
		}
	}
	
	
	//Now that we have the actual rect for the item we have to recalculate the report.
	report = getReport(actualRect, descriptor);
	
	
	//Fail if the report states that construction is not possible.
	if (!report.valid)
		return (ConstructionResult){false, "impossible"};
	//TODO: In the deluxe version, these things should return some highly detailed information on
	//what actually caused the construction to be rejected.
	
	
	//Calculate the costs of the construction by adding the total costs for the facility cells and
	//the total floor cell costs together.
	long costs = 0;
	costs += descriptor->price * report.additionalFacilityCellsRequired / descriptor->cells.x / descriptor->cells.y;
	costs += Item::descriptorForItemType(kFloorType)->price * report.additionalFloorCellsRequired;
	
	//Fail if we don't have enough money
	if (!tower->funds->hasSufficient(costs))
		return (ConstructionResult){false, "insufficient funds"};
	
	//Withdraw the required funds
	tower->funds->transfer(-costs);
	
	
	//In case this is a flexible width item, we now have to collapse with all items that are of the
	//same type.
	if (isFlexible) {
		//To do this we first have to get the list of items to collapse with.
		ItemSet collapseItems = report.collidesWith;
		
		//Since we might just be barely touching another object of the same type on the left or right we
		//also have to collapse with them.
		Cell * cellLeft = getCell(actualRect.minXminY() - int2(1, 0));
		Cell * cellRight = getCell(actualRect.maxXminY());
		Item * itemLeft = (cellLeft ? cellLeft->items[descriptor->category] : NULL);
		Item * itemRight = (cellRight ? cellRight->items[descriptor->category] : NULL);
		if (itemLeft) collapseItems.insert(itemLeft);
		if (itemRight) collapseItems.insert(itemRight);
		
		//Iterate through the collision items and add collapse with them
		for (ItemSet::iterator it = collapseItems.begin(); it != collapseItems.end(); it++) {
			
			//Assert that the item has the same type than us, since that must be the result of finding
			//the actual rect.
			assert((*it)->getType() == descriptor->type);
			
			//Find the union rect between the collison item and the actual rect.
			actualRect.unify((*it)->getRect());
			
			//Remove the old item which will be replaced by us.
			removeItem(*it);
		}
	}
	
	
	//Now we have to actually build the item. Pretty straightforward: Create the item and add it.
	Item * item = Item::make(tower, descriptor, actualRect);
	addItem(item);
	
	//Play the construction sound. Cadung-cadoush ^^.
	Audio::shared()->play(isFlexible ? flexibleConstructionSound : constructionSound);
	
	return (ConstructionResult){true, ""};
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

void TowerStructure::advance(double dt)
{
	//TODO: Mark us as needing an update. Later we should use the item's updateIfNeeded conditional
	//in conjunction with our updateIfNeeded conditional to propagate update needs properly.
	updateIfNeeded.setNeeded();
	
	//Advance the items
	ItemPointerSet items = getItems();
	for (ItemPointerSet::iterator it = items.begin(); it != items.end(); it++)
			(*it)->advance(dt);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

void TowerStructure::update()
{
	//Update the items
	ItemPointerSet items = getItems();
	for (ItemPointerSet::iterator it = items.begin(); it != items.end(); it++)
		(*it)->update();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

void TowerStructure::draw(rectd dirtyRect)
{
	//Find the items inside the dirty rect since they are the ones we need to draw. To do this we
	//first have to convert the dirty rect which is in world coordinates to cell coordinates.
	recti dirtyCells = worldToCell(dirtyRect);
	
	//Now we can ask for the items in that rect.
	ItemSet items = getItems(dirtyCells);
	
	//In the first pass we only draw the facility items. We have to do the facilities and transports
	//in two passes since we don't want any items to be drawn ontop of transportation.
	for (ItemSet::iterator it = items.begin(); it != items.end(); it++)
		if ((*it)->getCategory() == kFacilityCategory)
			(*it)->draw(dirtyRect);
	
	//In the second pass we only draw the transport items.
	for (ItemSet::iterator it = items.begin(); it != items.end(); it++)
		if ((*it)->getCategory() == kTransportCategory)
			(*it)->draw(dirtyRect);
}
