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
	decoration = new TowerDecoration(this);
	environment = new TowerEnvironment(this);
	funds = new TowerFunds(this);
	background = new TowerBackground(this);
	structure = new TowerStructure(this);
	
	//Make sure update needs get propagated to the tower from its subsystems.
	time->updateIfNeeded.parent = &updateIfNeeded;
	decoration->updateIfNeeded.parent = &updateIfNeeded;
	environment->updateIfNeeded.parent = &updateIfNeeded;
	funds->updateIfNeeded.parent = &updateIfNeeded;
	background->updateIfNeeded.parent = &updateIfNeeded;
	structure->updateIfNeeded.parent = &updateIfNeeded;
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
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

void Tower::advance(double dt)
{	
	//Advance the subsystems
	time->advance(dt);
	decoration->advance(dt);
	environment->advance(dt);
	funds->advance(dt);
	background->advance(dt);
	structure->advance(dt);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark State
//----------------------------------------------------------------------------------------------------

void Tower::update()
{
	//Update the subsystems
	time->updateIfNeeded();
	decoration->updateIfNeeded();
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
	
	//Draw the tower decoration
	decoration->draw(dirtyRect);
	
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
	if (decoration && decoration->sendEvent(event)) return true;
	if (environment && environment->sendEvent(event)) return true;
	if (funds && funds->sendEvent(event)) return true;
	if (structure && structure->sendEvent(event)) return true;
	if (background && background->sendEvent(event)) return true;
	return BasicResponder::sendEventToNextResponders(event);
}
