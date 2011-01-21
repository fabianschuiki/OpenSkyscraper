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

LobbyItem::LobbyItem(Tower * tower) : FacilityItem(tower, &descriptor),
updateEntrancesIfNeeded(this, &LobbyItem::updateEntrances, &updateIfNeeded)
{
	//The lobby doesn't need a ceiling since it is included in the lobby texture itself.
	setCeiling(false);
	
	//Initialize the outside entrances if this is the ground lobby.
	for (int i = 0; i < 2; i++) {
		Sprite * entrance = NULL;
		if (getMinFloor() == 0) {
			
			//Initialize the sprite
			entrance = new Sprite;
			
			//Setup its texture, texture rect and size
			entrance->texture = Texture::named("simtower/decoration/entrance");
			entrance->textureRect = rectd(i * 0.5, 0, 0.5, 1);
			entrance->rect.size = double2(56, 36);
		}
		
		//Keep the entrance we just create around for drawing
		outsideEntrances[i] = entrance;
	}
	
	//Initialize the inner entrance of the lobby.
	insideEntrance = new Sprite;
	insideEntrance->autoTexRectAttributes = (Sprite::kAutoX | Sprite::kRelative);
	insideEntrance->autoTexRectRelativeTo = rectd::kMinXminY;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark State
//----------------------------------------------------------------------------------------------------

void LobbyItem::update()
{
	FacilityItem::update();
	
	//Update the entrances if needed
	updateEntrancesIfNeeded();
}

void LobbyItem::updateBackground()
{
	FacilityItem::updateBackground();
	
	//Set the background sprite texture
	backgrounds[0]->texture = Texture::named(getLobbyTextureBaseName() + "/pattern");
	backgrounds[0]->autoTexRectAttributes = Sprite::kAutoX;
}

void LobbyItem::updateEntrances()
{
	//Calculate the location and size of the inside entrance
	insideEntrance->rect = getWorldRect();
	insideEntrance->rect.size.x = mini(56, insideEntrance->rect.size.x);
	
	//Load the appropriate texture for the inside entrance.
	insideEntrance->texture = Texture::named(getLobbyTextureBaseName() + "/entrance");
	
	//Position the outside entrances
	for (unsigned int i = 0; i < 2; i++) {
		Sprite * entrance = outsideEntrances[i];
		
		//Skip entrances that are not initialized
		if (!entrance)
			continue;
		
		//Load the appropriate entrance texture and set the texture rect.
		entrance->texture = Texture::named("simtower/decoration/entrance");
		entrance->textureRect = rectd(i * 0.5, 0, 0.5, 1);
		
		//Set the entrance location based on the index
		switch (i) {
			case 0: {
				entrance->rect.origin.x = getWorldRect().minX() - entrance->rect.size.x;
			} break;
			case 1: {
				entrance->rect.origin.x = getWorldRect().maxX();
			} break;
		}
	}
}

void LobbyItem::didChangeWorldRect()
{
	FacilityItem::didChangeWorldRect();
	
	//We have to reposition the entrances since our bounds changed
	updateEntrancesIfNeeded.setNeeded();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

string LobbyItem::getLobbyTextureBaseName()
{
	stringstream s;
	s << "simtower/lobby/single/";
	s << tower->environment->getLobbyStyle();
	s << "/";
	s << (getRect().minY() == 0 ? "ground" : "sky");
	return s.str();
}

void LobbyItem::draw(rectd dirtyRect)
{
	FacilityItem::draw(dirtyRect);
	
	//Draw the entrances
	for (int i = 0; i < 2; i++)
		if (outsideEntrances[i])
			outsideEntrances[i]->draw();
}

void LobbyItem::drawBackground(rectd dirtyRect)
{
	FacilityItem::drawBackground(dirtyRect);
	
	//Draw the inside entrance
	insideEntrance->draw();
}
