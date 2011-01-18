#include "facility.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

FacilityItem::FacilityItem(Tower * tower, ItemDescriptor * descriptor) : Item(tower, descriptor),
updateCeilingIfNeeded(this, &FacilityItem::updateCeiling, &updateIfNeeded),
updateLightingIfNeeded(this, &FacilityItem::updateLighting, &updateBackgroundIfNeeded)
{
	constructed = false;
	ceiling = true;
	variant = 0;
	lightsOn = false;
	
	//Update the lighting
	//updateLighting();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Ceiling
//----------------------------------------------------------------------------------------------------

bool FacilityItem::hasCeiling() const
{
	return ceiling;
}

void FacilityItem::setCeiling(bool flag)
{
	if (ceiling != flag) {
		ceiling = flag;
		
		//We have to update the background because we have to cut off a small amount off its top if
		//we have a ceiling.
		updateBackgroundIfNeeded.setNeeded();
		
		//Of course we also have to update the ceiling itself.
		updateCeilingIfNeeded.setNeeded();
	}
}

rectd FacilityItem::getCeilingRect()
{
	rectd r = getWorldRect();
	r.origin.y = r.maxY();
	r.size.y = tower->structure->ceilingHeight;
	r.origin.y -= r.size.y;
	return r;
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
		willChangeVariant(variant);
		this->variant = variant;
		didChangeVariant();
		updateBackgroundIfNeeded.setNeeded();
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Lighting
//----------------------------------------------------------------------------------------------------

bool FacilityItem::areLightsOn()
{
	return lightsOn;
}

void FacilityItem::setLightsOn(bool lo)
{
	if (lightsOn != lo) {
		lightsOn = lo;
		updateBackgroundIfNeeded.setNeeded();
	}
}



bool FacilityItem::shouldBeLitDueToTime()
{
	return tower->time->isBetween(7, 17);
}

bool FacilityItem::isLit()
{
	return (shouldBeLitDueToTime() || areLightsOn());
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark People
//----------------------------------------------------------------------------------------------------

void FacilityItem::didAddPerson(Person * person)
{
	Item::didAddPerson(person);
	person->setFloor(getRect().minY());
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

void FacilityItem::advance(double dt)
{
	Item::advance(dt);
	
	//Ask the facility to update its lighting state at twilight
	if (tower->time->checkDaily(7) || tower->time->checkDaily(17))
		updateLightingIfNeeded.setNeeded();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark State
//----------------------------------------------------------------------------------------------------

void FacilityItem::update()
{
	Item::update();
	
	//Update the ceiling and lighting if required
	updateCeilingIfNeeded();
	updateLightingIfNeeded();
}

void FacilityItem::updateBackground()
{	
	Item::updateBackground();
	
	//Adjust the topmost background's height if we have a ceiling
	if (hasCeiling()) {
		unsigned int index = (getNumFloors() - 1);
		if (backgrounds.count(index))
			backgrounds[index]->rect.size.y -= getCeilingRect().size.y;
	}
}

void FacilityItem::updateCeiling()
{
	//If we have a ceiling we have to load the appropriate ceiling texture
	if (hasCeiling())
		ceilingTexture = Texture::named("ceiling.png");
	
	//Otherwise we can just get rid of a potential ceiling texture
	else
		ceilingTexture = NULL;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

void FacilityItem::draw(rectd dirtyRect)
{
	Item::draw(dirtyRect);
	
	//Draw the ceiling if we have one
	if (hasCeiling())
		drawCeiling(dirtyRect);
}

void FacilityItem::drawCeiling(rectd dirtyRect)
{
	//Create a textured quad which we will use for drawing the ceiling
	TexturedQuad quad;
	quad.rect = getCeilingRect();
	quad.texture = ceilingTexture;
	
	//Automatically calculate the ceiling's texture rect
	quad.autogenerateTextureRect(true, false);
	
	//Draw the ceiling quad
	quad.draw();
}
