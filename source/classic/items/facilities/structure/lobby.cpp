#include "lobby.h"

using namespace OSS;
using namespace Classic;


ItemDescriptor LobbyItem::descriptor = {
	kLobbyType,
	kStructureGroup,
	kFacilityCategory,
	1,
	(kFlexibleWidthAttribute | kEvery15thFloorAttribute | kNotBelowGroundAttribute | kAllowedOnGroundAttribute),
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
	setHasCeiling(false);
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

string LobbyItem::getLobbyTextureBaseName()
{
	stringstream s;
	s << "simtower/lobby/single/";
	s << tower->environment->getLobbyStyle();
	s << "/";
	s << (getRect().minY() == 0 ? "ground" : "sky");
	return s.str();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Entrances
//----------------------------------------------------------------------------------------------------

void LobbyItem::initEntrances()
{
	//Setup the two entrance sprites
	for (int i = 0; i < 2; i++) {
		outsideEntrances[i].texture = Engine::Texture::named("simtower/decoration/entrance");
		outsideEntrances[i].textureRect = rectd(i * 0.5, 0, 0.5, 1);
	}
	
	//Update the entrances
	updateEntrances();
}

void LobbyItem::updateEntrances()
{
	//Calculate the entrance rects
	rectd outsideRects[2];
	for (int i = 0; i < 2; i++) {
		outsideRects[i].size = double2(56, 36);
		outsideRects[i].origin.y = getWorldRect().minY();
	}
	rectd insideRect(0, getWorldRect().minY(), std::min<int>(56, getWorldRect().size.x), 36);
	
	//Position the entrance rects horizontally
	outsideRects[0].origin.x = getWorldRect().minX() - outsideRects[0].size.x;
	outsideRects[1].origin.x = getWorldRect().maxX();
	insideRect.origin.x = getWorldRect().minX();
	
	//Adjust the inside rect's texture rect to compensate for rect widths < 56
	insideEntrance.textureRect.size.x = (insideRect.size.x / 56);
	
	//Set the entrance rects and decide whether they should be shown or not
	bool onGroundFloor = (getRect().origin.y == 0);
	for (int i = 0; i < 2; i++) {
		outsideEntrances[i].setRect(outsideRects[i]);
		outsideEntrances[i].setHidden(!onGroundFloor);
	}
	
	//Set the inside entrance
	insideEntrance.texture = Engine::Texture::named(getLobbyTextureBaseName() + "/entrance");
	insideEntrance.setRect(insideRect);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Basic Sprites
//----------------------------------------------------------------------------------------------------

void LobbyItem::initBackground()
{
	FacilityItem::initBackground();
		
	//Load the background sprite
	backgrounds[0].autoTexRectX = true;
	backgrounds[0].textureMode = Engine::Sprite::kRepeatTextureMode;
}

void LobbyItem::updateBackground()
{
	FacilityItem::updateBackground();
	
	//Update the background sprite
	backgrounds[0].texture = Engine::Texture::named(getLobbyTextureBaseName() + "/pattern");
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
		outsideEntrances[i].draw(visibleRect);
	if (!underConstruction)
		insideEntrance.draw(visibleRect);
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
