#include "decoration.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

TowerDecoration::TowerDecoration(Tower * tower) : tower(tower),
updateCraneIfNeeded(this, &TowerDecoration::updateCrane, &updateIfNeeded),
updateFireLaddersIfNeeded(this, &TowerDecoration::updateFireLadders, &updateIfNeeded)
{
	//Initialize the crane sprite
	craneSprite = new Sprite;
	craneSprite->texture = Texture::named("simtower/decoration/crane");
	craneSprite->rect.size = double2(36, 36);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark State
//----------------------------------------------------------------------------------------------------

void TowerDecoration::update()
{
	//Update the crane if needed
	updateCraneIfNeeded();
	
	//Update the fire ladders if needed
	updateFireLaddersIfNeeded();
}

void TowerDecoration::updateCrane()
{
	//Fetch the tower bounds.
	recti bounds = tower->structure->getBounds();
	
	//Calculate the topmost floor in the bounds which is maxY() - 1.
	int maxFloor = bounds.maxY() - 1;
	
	//Get the rect of this floor.
	rectd floorRect = tower->structure->getWorldFloorRect(maxFloor);
	
	//We want to position the crane ontop of the floor rect, attached to its left.
	craneSprite->rect.origin = floorRect.minXmaxY();
	craneSprite->rect.origin.x -= 4;
	
	//Hide the crane if it sits on the ground :)
	craneSprite->hidden = (craneSprite->rect.minY() <= 0);
}

void TowerDecoration::updateFireLadders()
{
	//Iterate through the floors that need their fire ladders updated.
	for (vector<int>::iterator it = fireLaddersNeedingUpdate.begin();
		 it != fireLaddersNeedingUpdate.end(); it++) {
		
		//Don't bother building fire ladders for floors below ground
		if (*it < 0) continue;
		
		//Get the pair of fire ladder sprites for this floor
		SpritePair * sprites = &fireLadders[*it];
		
		//Initialize the sprites if required
		if (!sprites->first) {
			sprites->first = makeNewFireLadderSprite();
			sprites->first->textureRect.origin.x = 0;
		}
		if (!sprites->second) {
			sprites->second = makeNewFireLadderSprite();
			sprites->second->textureRect.origin.x = 0.5;
		}
		
		//Fetch the floor's rect.
		rectd floorRect = tower->structure->getWorldFloorRect(*it);
		
		//Position the left ladder
		sprites->first->rect.origin = floorRect.minXminY();
		sprites->first->rect.origin.x -= sprites->first->rect.size.x;
		
		//Position the right ladder
		sprites->second->rect.origin = floorRect.maxXminY();
	}
	
	//Clear the set of fire ladder floors that need to be updated.
	fireLaddersNeedingUpdate.clear();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

Sprite * TowerDecoration::makeNewFireLadderSprite()
{
	Sprite * s = new Sprite;
	s->rect.size = tower->structure->cellToWorld(int2(3, 1));
	s->texture = Texture::named("simtower/decoration/stairs");
	s->textureRect = rectd(0, 0, 0.5, 1);
	return s;
}

void TowerDecoration::draw(rectd dirtyRect)
{
	//Draw the crane
	drawCrane(dirtyRect);
	
	//Draw the fire ladders
	drawFireLadders(dirtyRect);
}

void TowerDecoration::drawCrane(rectd dirtyRect)
{
	craneSprite->draw();
}

void TowerDecoration::drawFireLadders(rectd dirtyRect)
{
	//Calculate the lowest and highest visible floor.
	recti dirtyCells = tower->structure->worldToCell(dirtyRect);
	int lowest = dirtyCells.minY();
	int hightest = dirtyCells.maxY() - 1;
	
	//Iterate through the visible floors and draw the fire ladders where appropriate.
	for (int i = lowest; i <= hightest; i++) {
		
		//Skip this floor if we don't have a pair of ladders for it.
		if (!fireLadders.count(i))
			continue;
		
		//Fetch the pair of ladders
		SpritePair ladders = fireLadders[i];
		
		//Draw the ladders if visible
		if (ladders.first->rect.intersectsRect(dirtyRect))
			ladders.first->draw();
		if (ladders.second->rect.intersectsRect(dirtyRect))
			ladders.second->draw();
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Events
//----------------------------------------------------------------------------------------------------

void TowerDecoration::eventBoundsChanged(Event * event)
{
	//If the crane intersects the tower bounds, we have to move it since we want it to sit on top
	//of the tower where it normally doesn't intersect.
	if (craneSprite->rect.minY() < tower->structure->getWorldBounds().maxY())
		updateCraneIfNeeded.setNeeded();
}

void TowerDecoration::eventFloorRangeChanged(FloorEvent * event)
{
	//Add the floor to the ones requiring an update.
	fireLaddersNeedingUpdate.push_back(event->floor);
	
	//Mark the fire ladders as to be updated.
	updateFireLaddersIfNeeded.setNeeded();
}
