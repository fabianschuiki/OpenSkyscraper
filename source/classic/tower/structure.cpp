#include "structure.h"

#include "../items/item.h"
#include "tower.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

TowerStructure::TowerStructure(Tower * tower) : Engine::Object(), tower(tower)
{
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
		assert(*slot != item && "not all cells covered by the item are assigned to it!");
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
	bzero(&report, sizeof(report));
	if (!descriptor)
		return report;
	
	//Check the attributes if they are fulfilled
	report.unfulfilledAttributes = descriptor->attributes;
	
	//Only on ground
	if ((descriptor->attributes & kAllowedOnGroundAttribute) || rect.maxY() <= 0 || rect.minY() > 0)
		report.unfulfilledAttributes &= ~(kAllowedOnGroundAttribute);
	
	//Above/below ground
	if (!(descriptor->attributes & kNotAboveGroundAttribute) || rect.maxY() <= 0)
		report.unfulfilledAttributes &= ~(kNotAboveGroundAttribute);
	if (!(descriptor->attributes & kNotBelowGroundAttribute) || rect.minY() > 0)
		report.unfulfilledAttributes &= ~(kNotBelowGroundAttribute);
	
	//Every 15th floor
	if (!(descriptor->attributes & kEvery15thFloorAttribute) || (rect.minY() % 15) == 0)
		report.unfulfilledAttributes &= ~(kNotAboveGroundAttribute);
	
	
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
		localMask = rectmaski(&rect);
	else
		localMask.offset(rect.origin);
	
	
	//Get all the items that are affected by the rectangle
	ItemSet items = getItems(rect);
	
	//Iterate through the items and check whether they collide in any form with the item
	for (ItemSet::iterator it = items.begin(); it != items.end(); it++)
		if ((*it)->getCategory() == descriptor->category)
			if (descriptor->mask.intersectsRect((*it)->getRect()))
				report.collidesWith.insert(*it);
	
	
	//Analyze the cells to find how many additional cells will be required
	CellAnalysis analysis = analyseCells(localMask);
	report.additionalFacilityCellsRequired = (localMask.area() - analysis.facility);
	report.additionalFloorCellsRequired = (report.additionalFacilityCellsRequired - analysis.floor);
	
	
	//Facilities and transports have different requirements
	report.validForFacility = (!report.unfulfilledAttributes &&
							   report.adjacentCellsValid &&
							   analysis.facility == 0);
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
