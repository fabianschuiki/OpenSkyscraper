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

void Tower::renderBackground(rectd visibleRect)
{
	//Calculate the rectangles
	rectd groundRect = visibleRect;
	groundRect.size.y = -groundRect.origin.y;
	rectd skyRect = visibleRect;
	skyRect.size.y += skyRect.origin.y;
	skyRect.origin.y = 0;
	
	//Load the ground sprite if necessary
	if (!groundSprite) {
		groundSprite = new Sprite;
		groundSprite->texture = Texture::named("049.bmp");
		groundSprite->rect = rectd(-800, -360, 1600, 360);
		groundSprite->textureMode = Sprite::kRepeatTextureMode;
		groundSprite->autoTexRectX = true;
		groundSprite->autoTexRectY = true;
	}
	groundSprite->draw(visibleRect);
	
	//Load the sky sprites if necessary
	for (int i = 0; i < 10; i++) {
		if (!skySprites[i]) {		
			char n[16];
			sprintf(n, "%03i.bmp", i + 50);
			skySprites[i] = new Sprite;
			skySprites[i]->texture = Texture::named(n);
			skySprites[i]->rect = rectd(-800, i * 360, 1600, 360);
			skySprites[i]->textureMode = Sprite::kRepeatTextureMode;
			skySprites[i]->autoTexRectX = true;
		}
		skySprites[i]->draw(visibleRect);
	}
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
