#include "facility.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

FacilityItem::FacilityItem(Tower * tower, ItemDescriptor * descriptor) : Item(tower, descriptor)
{
	hasCeiling = true;
	variant = 0;
	lit = false;
	
	//Update the lighting
	updateLighting();
}





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
	ceiling.textureMode = Engine::Sprite::kRepeatTextureMode;
	ceiling.texture = Engine::Texture::named("ceiling.png");
}


void FacilityItem::updateCeiling()
{
	if (!hasCeiling) return;
	
	//Calculate the ceiling rect
	rectd rect = getWorldRect();
	rect.origin.y = rect.maxY();
	rect.size.y = 12;
	rect.origin.y -= rect.size.y;
	ceiling.setRect(rect);
}

bool FacilityItem::getHasCeiling() const
{
	return hasCeiling;
}

void FacilityItem::setHasCeiling(bool flag)
{
	hasCeiling = flag;
}

void FacilityItem::updateBackground()
{	
	Item::updateBackground();
	
	//Adjust the topmost background's height if we have a ceiling
	if (hasCeiling) {
		Engine::Sprite * background = &backgrounds[getNumFloors() - 1];
		rectd rect = background->getRect();
		rect.size.y -= ceiling.getRect().size.y;
		background->setRect(rect);
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Variant
//----------------------------------------------------------------------------------------------------

unsigned int FacilityItem::getVariant() const
{
	return variant;
}

void FacilityItem::setVariant(const unsigned int variant)
{
	if (this->variant != variant) {
		this->variant = variant;
		onChangeVariant();
	}
}

void FacilityItem::onChangeVariant()
{
	updateBackground();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Lighting
//----------------------------------------------------------------------------------------------------

bool FacilityItem::isLit() const
{
	return lit;
}

void FacilityItem::setLit(bool lit)
{
	if (this->lit != lit) {
		this->lit = lit;
		onChangeLit();
	}
}

bool FacilityItem::shouldBeLitDueToTime()
{
	return (tower->time >= 7 && tower->time < 17);
}

void FacilityItem::updateLighting()
{
	setLit(shouldBeLitDueToTime());
}

void FacilityItem::onChangeLit()
{
	updateBackground();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

void FacilityItem::advance(double dt)
{
	Item::advance(dt);
	
	//Ask the facility to update its lighting state at twilight
	if (tower->checkTime(7) || tower->checkTime(17))
		updateLighting();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

void FacilityItem::draw(rectd visibleRect)
{
	Item::draw(visibleRect);
	
	//Draw the ceiling sprite
	if (hasCeiling && (!underConstruction || !drawFlexibleConstruction))
		ceiling.draw(visibleRect);
}
