#include "lobby.h"

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
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

LobbyItem::LobbyItem(Tower * tower) : FacilityItem(tower, &descriptor)
{
}

void LobbyItem::init()
{
	FacilityItem::init();
	initEntrances();
}

void LobbyItem::update()
{
	FacilityItem::update();
	updateEntrances();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Entrances
//----------------------------------------------------------------------------------------------------

void LobbyItem::initEntrances()
{
	//Setup the two entrance sprites
	for (int i = 0; i < 2; i++) {
		entrances[i].texture = Texture::named("simtower/decoration/entrance");
		entrances[i].textureRect = rectd(i * 0.5, 0, 0.5, 1);
	}
	
	//Update the entrances
	updateEntrances();
}

void LobbyItem::updateEntrances()
{
	//Calculate the entrance rects
	rectd rects[2];
	for (int i = 0; i < 2; i++) {
		rects[i].size = double2(56, 36);
		rects[i].origin.y = getWorldRect().minY();
	}
	
	//Position the entrance rects horizontally
	rects[0].origin.x = getWorldRect().minX() - rects[0].size.x;
	rects[1].origin.x = getWorldRect().maxX();
	
	//Set the entrance rects and decide whether they should be shown or not
	bool onGroundFloor = (getRect().origin.y == 0);
	for (int i = 0; i < 2; i++) {
		entrances[i].setRect(rects[i]);
		entrances[i].setHidden(!onGroundFloor);
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Basic Sprites
//----------------------------------------------------------------------------------------------------

void LobbyItem::initCeiling()
{
	FacilityItem::initCeiling();
	
	//We need the strong ceiling for the lobby
	ceiling.texture = Texture::named("ceiling-strong.png");
}

void LobbyItem::initBackground()
{
	FacilityItem::initBackground();
		
	//DEBUG: Load the debug background sprite
	backgrounds[0].autoTexRectX = true;
	backgrounds[0].textureMode = Sprite::kRepeatTextureMode;
	backgrounds[0].texture = Texture::named("lobby2_debug.png");
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

void LobbyItem::draw(rectd visibleRect)
{
	//Draw the item
	FacilityItem::draw(visibleRect);
	
	//Draw the entrances
	for (int i = 0; i < 2; i++)
		entrances[i].draw(visibleRect);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Notifications
//----------------------------------------------------------------------------------------------------

void LobbyItem::onChangeLocation()
{
	FacilityItem::onChangeLocation();
	updateEntrances();
}
