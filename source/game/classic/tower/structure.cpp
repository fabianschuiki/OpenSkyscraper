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
	//Initialize the members to zero
	constructionsHalted = false;
	
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
	flexibleConstructionSound->volume = 0.5;
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
#pragma mark Bounds
//----------------------------------------------------------------------------------------------------

const recti & TowerStructure::getBounds()
{
	return bounds;
}

void TowerStructure::setBounds(const recti & rect)
{
	if (bounds != rect) {
		bounds = rect;
		tower->sendEvent(new Event(Event::kBoundsChanged));
	}
}

void TowerStructure::extendBounds(const recti & rect)
{
	//Create a working copy of the rect.
	recti r = rect;
	
	//If our current bounds are not empty, we extend the working rect by them.
	if (bounds.area() > 0)
		r.unify(bounds);
	
	//Set the new bounds
	setBounds(r);
}



rectd TowerStructure::getWorldBounds()
{
	return cellToWorld(getBounds());
}



TowerStructure::FloorRange TowerStructure::getFloorRange(int floor)
{
	return floorRanges[floor];
}

void TowerStructure::setFloorRange(int floor, FloorRange range)
{
	if (floorRanges[floor] != range) {
		floorRanges[floor] = range;
		tower->sendEvent(new FloorEvent(Event::kFloorRangeChanged, floor));
	}
}

void TowerStructure::extendFloorRange(int floor, const recti & rect)
{
	//Get the current floor's range
	FloorRange fr = floorRanges[floor];
	
	//Check whether the floor is empty, i.e. the minX and maxX are identical.
	bool empty = (fr.minX == fr.maxX);
	
	//Add the rect's minX and maxX to the range appropriately.
	if (empty || rect.minX() < fr.minX)
		fr.minX = rect.minX();
	if (empty || rect.maxX() > fr.maxX)
		fr.maxX = rect.maxX();
	
	//Set the new floor range
	setFloorRange(floor, fr);
}


recti TowerStructure::getFloorRect(int floor)
{
	recti r(0, floor, 0, 1);
	FloorRange fr = getFloorRange(floor);
	r.origin.x = fr.minX;
	r.size.x = fr.maxX - fr.minX;
	return r;
}

rectd TowerStructure::getWorldFloorRect(int floor)
{
	return cellToWorld(getFloorRect(floor));
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
	CellSet res;
	for (CellSet::iterator it = cells.begin(); it != cells.end(); it++)
		if (rectmask.containsPoint((*it)->location))
			res.insert(*it);
	return res;
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
	
	//Extend the tower bounds.
	extendBounds(item->getRect());
	
	//Extend the floor ranges.
	for (int i = item->getMinFloor(); i <= item->getMaxFloor(); i++)
		extendFloorRange(i, item->getRect());
	
	//Get the cells the item covers
	CellSet cells = getCells(item->getOccupiedRectMask(), true);
	
	//Set the item in each cell's item slot
	for (CellSet::iterator it = cells.begin(); it != cells.end(); it++) {
		Cell * cell = *it;
		Item ** slot = &cell->items[item->getCategory()];
		assert(*slot == NULL && "occupied by another item. check before adding.");
		*slot = item;
	}
	
	//Update the empty floor cells.
	for (int i = item->getMinFloor(); i <= item->getMaxFloor(); i++)
		recalculateEmptyFloorRectsOnFloor(i);
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
	
	//Update the empty floor cells.
	for (int i = item->getMinFloor(); i <= item->getMaxFloor(); i++)
		recalculateEmptyFloorRectsOnFloor(i);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Empty Floors
//----------------------------------------------------------------------------------------------------

void TowerStructure::recalculateEmptyFloorRectsOnFloor(int floor)
{
	OSSObjectLog << "on " << floor << std::endl;
	vector<recti> rects;
	
	//Iterate through all cells on this floor and find adjacent empty floor cells.
	FloorRange range = getFloorRange(floor);
	recti rect;
	
	for (int i = range.minX; i < range.maxX; i++) {
		
		//Fetch this cell, skipping if it is invalid.
		Cell * cell = getCell(int2(i, floor), false);
		if (!cell)
			continue;
		
		//If the rect is not of valid width, keep moving it along with the iteration.
		if (!rect.size.x)
			rect.origin.x = i;
		
		//If the cell is an empty floor, adjust the current rect to cover it.
		if (!cell->items[kFacilityCategory]) {
			rect.origin.y = floor;
			rect.size.y = 1;
			rect.size.x = (i + 1 - rect.origin.x);
		}
		
		//Otherwise check whether the accumulated rect was valid, in which case we add it to the
		//list.
		else if (rect.size.x) {
			rects.push_back(rect);
			
			//Reset the rect.
			rect.size.x = 0;
		}
	}
	
	//If there is some valid rect left in the accumulator, add it to the list too.
	if (rect.size.x)
		rects.push_back(rect);
	
	//Dump the content of the rects array.
	for (int i = 0; i < rects.size(); i++)
		OSSObjectLog << " - " << rects[i].description() << std::endl;
	
	//Replace this floor's rects.
	emptyFloorRects[floor] = rects;
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



bool TowerStructure::containsItem(Item * item)
{
	return items.count(item);
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
	
	//Facility limitations
	if (descriptor->category == kFacilityCategory) {
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
	}
	
	
	//Check if the items above/below are valid
	recti rectAbove(rect.origin.x, rect.maxY(), rect.size.x, 1);
	recti rectBelow(rect.origin.x, rect.minY() - 1, rect.size.x, 1);
	CellAnalysis analysisAbove = analyseCells(rectAbove);
	CellAnalysis analysisBelow = analyseCells(rectBelow);
	report.cellsAboveValid = (analysisAbove.facility + analysisAbove.floor == rectAbove.size.x);
	report.cellsBelowValid = (analysisBelow.facility + analysisBelow.floor == rectBelow.size.x);
	
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
	
	
	//Get all the items that are affected by the mask
	ItemSet items = getItems(localMask);
	
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
								/*report.adjacentCellsValid &&*/
								analysis.transport == 0 &&
								report.additionalFacilityCellsRequired == 0);
	
	//Check the overall validity of the location based on the descriptor's category
	report.valid = false;
	switch (descriptor->category) {
		case kFacilityCategory:		report.valid = report.validForFacility; break;
		case kTransportCategory:	report.valid = report.validForTransport; break;
	}
	
	//For each floor of the item, check whether there is something else on this floor that we need
	//to connect to using empty floor cells.
	OSSObjectLog << "--- starting analysis ---" << std::endl;
	int adjacentFloorArea = 0;
	for (int floor = rect.minY(); floor < rect.maxY(); floor++) {
		OSSObjectLog << "checking floor " << floor << std::endl;
		
		//Fetch the local floor range.
		FloorRange range = getFloorRange(floor);
		
		//If there is nothing on this floor, skip ahead.
		if (!range.length())
			continue;
		
		//Find the rect that lies between the item rect and the floor range.
		recti gap(0, floor, 0, 1);
		if (range.maxX <= rect.minX()) {
			gap.origin.x = range.maxX;
			gap.size.x = rect.minX() - gap.origin.x;
		}
		if (range.minX >= rect.maxX()) {
			gap.origin.x = rect.maxX();
			gap.size.x = range.minX - gap.origin.x;
		}
		
		//If the gap has an area > 0, add it to the additional adjacent floor cells mask.
		if (gap.area()) {
			adjacentFloorArea += gap.area();
			report.additionalAdjacentFloorCellRects.push_back(gap);
		}
		
		OSSObjectLog << "range [" << range.minX << ", " << range.maxX << "], gap " << gap.description() << std::endl;
	}
	OSSObjectLog << "additional floor area " << adjacentFloorArea << std::endl;
	
	//Add the adjacent floor cells to the overall additional floor cell count.
	report.additionalFloorCellsRequired += adjacentFloorArea;
	
	return report;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Item Construction
//----------------------------------------------------------------------------------------------------

bool TowerStructure::areConstructionsHalted()
{
	return constructionsHalted;
}

void TowerStructure::setConstructionsHalted(bool ch)
{
	constructionsHalted = ch;
}

TowerStructure::ConstructionResult TowerStructure::constructItem(ItemDescriptor * descriptor,
																 recti rect)
{
	return constructItem(descriptor, rect, rect);
}

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
	//TODO: make this work for transport items
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
			//Commented line below, mark this because needs future fixing. It caused a crash when building a floor
			//assert((*it)->getType() == descriptor->type);
			
			//Find the union rect between the collison item and the actual rect.
			actualRect.unify((*it)->getRect());
			
			//Remove the old item which will be replaced by us.
			removeItem(*it);
		}
	}
	
	//Fill the gaps to existing floors.
	vector<recti> cellRects = report.additionalAdjacentFloorCellRects;
	for (vector<recti>::iterator it = cellRects.begin(); it != cellRects.end(); it++) {
		
		//Create the required cells.
		CellSet cells = getCells(*it, true);
		
		//Iterate through the cells and set the facility category item to NULL which represents an
		//empty floor.
		for (CellSet::iterator c = cells.begin(); c != cells.end(); c++)
			(*c)->items[kFacilityCategory] = NULL;
	}
	
	
	//Now we have to actually build the item. Pretty straightforward: Create the item and add it.
	Item * item = Item::make(tower, descriptor, actualRect);
	addItem(item);
	
	//Play the construction sound. Cadung-cadoush ^^.
	Audio::shared()->play(isFlexible ? flexibleConstructionSound : constructionSound);
	
	//If we built a transport item, send an event so the reachabilities may be adjusted.
	if (descriptor->category == kTransportCategory)
		tower->sendEvent(new ItemEvent<TransportItem>(Event::kTransportIncreased, (TransportItem *)item));
	
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
#pragma mark State
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
	
	//Draw the empty floors.
	drawEmptyFloors(dirtyCells);
	
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

void TowerStructure::drawEmptyFloors(recti dirtyCells)
{
	//Assemble floor quad.
	TexturedQuad floorQuad;
	floorQuad.color = (color4d){0.3, 0.3, 0.3, 1};
	
	//Assemble the ceiling quad.
	TexturedQuad ceilingQuad;
	ceilingQuad.texture = Texture::named("ceiling.png");
	
	//Draw the empty floors rects.
	for (int floor = dirtyCells.minY(); floor < dirtyCells.maxY(); floor++) {
		
		//First fetch the empty floor rects on this floor,
		const vector<recti> & rects = emptyFloorRects[floor];
		
		//Iterate through them and draw each.
		for (vector<recti>::const_iterator it = rects.begin(); it != rects.end(); it++) {
			
			//Skip rects out of bounds.
			if (!dirtyCells.intersectsRect(*it))
				continue;
			
			//Adjust the quad rectangles.
			floorQuad.rect = cellToWorld(*it);
			floorQuad.rect.size.y -= ceilingHeight;
			ceilingQuad.rect = floorQuad.rect;
			ceilingQuad.rect.origin.y = floorQuad.rect.maxY();
			ceilingQuad.rect.size.y = ceilingHeight;
			
			//Draw the rectangles.
			floorQuad.draw();
			ceilingQuad.draw();
		}
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Events
//----------------------------------------------------------------------------------------------------

bool TowerStructure::sendEventToNextResponders(OSS::Event * event)
{
	for (ItemPointerSet::iterator it = items.begin(); it != items.end(); it++)
		if ((*it) && (*it)->sendEvent(event)) return true;
	return GameObject::sendEventToNextResponders(event);
}
