#include "item.h"
#include "items.h"

using namespace OSS;


static Item::Descriptor floorItemDescriptor = {
	Item::kFloorType,
	Item::kStructureGroup,
	Item::kFacilityCategory,
	1,
	(Item::kFlexibleWidthAttribute),
	500,
	int2(1, 1),
	int2(1, 1)
};





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

Item * Item::createNew(Item::Descriptor * descriptor, recti rect, unsigned int itemID)
{
	Item * instance = NULL;
	
	//Allocate the new item according to the descriptor type
	switch (descriptor->type) {
			//Structure
		case Item::kLobbyType:		instance = new LobbyItem; break;
	}
	
	//Catch errors
	assert(instance != NULL);
	
	//Initialize the instance
	instance->itemID = itemID;
	instance->descriptor = descriptor;
	instance->rect = rect;
	instance->onPrepare();
	
	return instance;
}

Item::Item()
{
	constructionProgress = 0;
}

Item::~Item()
{
	ceilingSprite = NULL;
	backgroundSprite = NULL;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Descriptors
//----------------------------------------------------------------------------------------------------

Item::Descriptor * Item::descriptorForItemType(Item::Type itemType)
{
	switch (itemType) {
		case Item::kLobbyType:	return &LobbyItem::descriptor; break;
		case Item::kFloorType:	return &floorItemDescriptor; break;
	}
	return NULL;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

void Item::advance(double dt)
{
	//Simulate the construction
	if (constructionProgress < 1.0) {
		constructionProgress += dt * 1.0;
		
		if (constructionProgress < 0.1)
			switchToConstructionState(0);
		else if (constructionProgress < 1.0)
			switchToConstructionState(1);
		else
			switchToConstructionState(2);
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Rendering
//----------------------------------------------------------------------------------------------------

void Item::draw(rectd visibleRect)
{
	//Position the sprites
	backgroundSprite->rect = worldRect;
	ceilingSprite->rect = worldRect;
	ceilingSprite->rect.size.y = 12;
	backgroundSprite->rect.size.y -= ceilingSprite->rect.size.y;
	ceilingSprite->rect.origin.y = backgroundSprite->rect.maxY();
	
	//Draw the construction
	if (constructionSprite) {
		if (constructionState == 0)
			constructionSprite->rect = worldRect;
		else
			constructionSprite->rect = backgroundSprite->rect;
		constructionSprite->draw(visibleRect);
	}
	
	//Draw the sprites
	if (constructionState >= 2)
		backgroundSprite->draw(visibleRect);
	if (constructionState >= 1)
		ceilingSprite->draw(visibleRect);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Notifications
//----------------------------------------------------------------------------------------------------

void Item::onPrepare()
{
	//Initialize the construction sprite
	constructionState = 1;
	switchToConstructionState(0);
	
	//Initialize the background sprite
	backgroundSprite = new Sprite;
	
	//Initialize the ceiling sprite
	ceilingSprite = new Sprite;
	ceilingSprite->autoTexRectX = true;
	ceilingSprite->textureMode = Sprite::kRepeatTextureMode;
	ceilingSprite->texture = Texture::named(descriptor->type == Item::kLobbyType ? "ceiling-strong.png" : "ceiling.png");
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Notifications
//----------------------------------------------------------------------------------------------------

void Item::switchToConstructionState(unsigned int state)
{
	if (constructionState == state) return;
	
	if (!constructionSprite) {
		constructionSprite = new Sprite;
		constructionSprite->textureMode = Sprite::kRepeatTextureMode;
		constructionSprite->autoTexRectX = true;
	}
	
	switch (state) {
		case 0: {
			constructionSprite->texture = Texture::named("211.bmp");
		} break;
		case 1: {
			constructionSprite->texture = Texture::named("212.bmp");
		} break;
		case 2: {
			constructionSprite = NULL;
		} break;
	}
	
	constructionState = state;
}
