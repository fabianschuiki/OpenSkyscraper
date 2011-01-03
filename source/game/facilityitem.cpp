#include "facilityitem.h"

using namespace OSS;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Basic Sprites
//----------------------------------------------------------------------------------------------------

void FacilityItem::initBasicSprites()
{
	Item::initBasicSprites();
	initCeiling();
}

void FacilityItem::updateBasicSprites()
{
	Item::updateBasicSprites();
	updateCeiling();
}

void FacilityItem::initCeiling()
{
	ceiling.autoTexRectX = true;
	ceiling.textureMode = Sprite::kRepeatTextureMode;
	ceiling.texture = Texture::named("ceiling.png");
}


void FacilityItem::updateCeiling()
{
	//Calculate the ceiling rect
	rectd rect = getWorldRect();
	rect.origin.y = rect.maxY();
	rect.size.y = 12;
	rect.origin.y -= rect.size.y;
	ceiling.setRect(rect);
}

void FacilityItem::updateBackground()
{	
	Item::updateBackground();
	
	//Adjust the topmost background's height
	Sprite * background = &backgrounds[getNumFloors() - 1];
	rectd rect = background->getRect();
	rect.size.y -= ceiling.getRect().size.y;
	background->setRect(rect);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

void FacilityItem::draw(rectd visibleRect)
{
	Item::draw(visibleRect);
	
	//Draw the ceiling sprite
	if (!underConstruction || !drawFlexibleConstruction)
		ceiling.draw(visibleRect);
}
