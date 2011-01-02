#include "item.h"
#include "items.h"
#include "tower.h"

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
			
			//Office
		case Item::kOfficeType:		instance = new OfficeItem; break;
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
	itemID = 0;
	descriptor = NULL;
	
	constructionWorkerUpdateTimer = 0;
	constructionProgress = 0;
	underConstruction = false;
	drawFlexibleConstruction = false;
}

Item::~Item()
{
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
			
		case Item::kOfficeType:	return &OfficeItem::descriptor; break;
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
		//Increase the construction worker update timer
		constructionWorkerUpdateTimer += dt;
		if (constructionWorkerUpdateTimer > 0.1) {
			constructionWorkerUpdateTimer -= 0.1;
			updateConstructionWorkerSprites();
		}
		
		//Advance the construction progress
		if (!tower->getConstructionsHalted()) {
			constructionProgress += dt * 1.0;
			if (constructionProgress >= 1.0 || drawFlexibleConstruction)
				setUnderConstruction(false);
		}
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
		constructionSprite->rect = (drawFlexibleConstruction ? worldRect : backgroundSprite->rect);
		constructionSprite->autoTexRelativeX = !drawFlexibleConstruction;
		constructionSprite->draw(visibleRect);
	}
	
	//Draw the construction workers
	if (!drawFlexibleConstruction)
		for (int i = 0; i < 3; i++)
			if (constructionWorkerSprite[i])
				constructionWorkerSprite[i]->draw(visibleRect);
	
	//Draw the sprites
	if (!underConstruction)
		backgroundSprite->draw(visibleRect);
	if (!underConstruction || !drawFlexibleConstruction)
		ceilingSprite->draw(visibleRect);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Notifications
//----------------------------------------------------------------------------------------------------

void Item::onPrepare()
{	
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
#pragma mark Uncategorized
//----------------------------------------------------------------------------------------------------

void Item::setUnderConstruction(bool uc)
{
	//Ignore if the construction state didn't change
	if (underConstruction == uc) return;
	underConstruction = uc;
	
	//Switch to construction mode
	if (underConstruction) {
		//Setup the construction sprite if required
		if (!constructionSprite) {
			constructionSprite = new Sprite;
			constructionSprite->textureMode = Sprite::kRepeatTextureMode;
			constructionSprite->autoTexRectX = true;
			
			//Depending on whether the item is draggable the construction sprite differs
			drawFlexibleConstruction = (this->descriptor->attributes & kFlexibleWidthAttribute);
			std::string textureName = "simtower/construction/";
			textureName += (drawFlexibleConstruction ? "floor" : "facility");
			constructionSprite->texture = Texture::named(textureName);
		}
		
		//Setup the construction
		constructionProgress = 0;
		
		//Setup the construction worker sprites
		for (int i = 0; i < 3; i++) {
			if (!constructionWorkerSprite[i] && rect.size.y > i) {
				Sprite * sprite = new Sprite;
				sprite->rect.size = double2(16, 24);
				sprite->texture = Texture::named("simtower/construction/workers");
				sprite->textureRect.size = double2(1.0 / 6, 1);
				constructionWorkerSprite[i] = sprite;
			}
		}
		updateConstructionWorkerSprites();
	}
	
	//Switch to constructed mode
	else {
		constructionSprite = NULL;
		constructionProgress = 1.0;
		
		//Destroy construction worker sprites
		for (int i = 0; i < 3; i++)
			constructionWorkerSprite[i] = NULL;
	}
}

void Item::updateConstructionWorkerSprites()
{
	for (int i = 0; i < 3; i++) {
		Sprite * sprite = constructionWorkerSprite[i];
		if (!sprite) continue;
		
		//Position the sprite
		int2 newCoords(rand() % (rect.size.x - 1), i);
		sprite->rect.origin = tower->convertCellToWorldCoordinates(newCoords + rect.origin);
		
		//Choose a new texture area
		sprite->textureRect.origin.x = (rand() % 6) * sprite->textureRect.size.x;
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Accessors
//----------------------------------------------------------------------------------------------------

void Item::setTower(Tower * tower)
{
	this->tower = tower;
}
