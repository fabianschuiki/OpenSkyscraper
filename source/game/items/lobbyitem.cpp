#include "lobbyitem.h"

using namespace OSS;


Item::Descriptor LobbyItem::descriptor = {
	Item::kLobbyType,
	Item::kStructureGroup,
	Item::kFacilityCategory,
	1,
	(Item::kFlexibleWidthAttribute | Item::kEvery15thFloorAttribute | Item::kNotBelowGroundAttribute),
	5000, 
	int2(1, 1),
	int2(4, 1)
};





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Rendering
//----------------------------------------------------------------------------------------------------

void LobbyItem::draw(rectd visibleRect)
{
	Item::draw(visibleRect);
	
	//Reposition the entrances if there are any
	if (entranceSprites[0]) {
		entranceSprites[0]->rect.origin.x = worldRect.minX() - entranceSprites[0]->rect.size.x;
	}
	if (entranceSprites[1]) {
		entranceSprites[1]->rect.origin.x = worldRect.maxX();
	}
	for (int i = 0; i < 2; i++) {
		if (entranceSprites[i]) {
			entranceSprites[i]->rect.origin.y = worldRect.minY();
			entranceSprites[i]->draw(visibleRect);
		}
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Notifications
//----------------------------------------------------------------------------------------------------

void LobbyItem::onPrepare()
{
	Item::onPrepare();
	
	//If this is the ground lobby we need some entrances
	if (rect.origin.y == 0) {
		for (int i = 0; i < 2; i++) {
			entranceSprites[i] = new Sprite;
			entranceSprites[i]->texture = Texture::named("067.bmp");
			entranceSprites[i]->textureRect = rectd(0, 0, 0.5, 1);
			entranceSprites[i]->rect = rectd(0, 0, 56, 36);
		}
		
		//Set the texture rect
		entranceSprites[1]->textureRect.origin.x = 0.5;
	}
}
