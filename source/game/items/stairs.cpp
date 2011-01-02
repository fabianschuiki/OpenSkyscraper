#include "stairs.h"
#include "../tower.h"

using namespace OSS;


Item::Descriptor StairsItem::descriptor = {
	Item::kStairsType,
	Item::kStructureGroup,
	Item::kTransportCategory,
	1,
	(0),
	5000,
	int2(8, 2),
	int2(8, 2),
	rectmaski(new recti(0, 0, 4, 1), new recti(4, 1, 4, 1), NULL)
};





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

StairsItem::StairsItem(Tower * tower) : Item(tower, &descriptor)
{
	animationFrame = 0;
	animationProgress = 0;
	inUse = true;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Animation
//----------------------------------------------------------------------------------------------------

unsigned int StairsItem::getAnimationFrame()
{
	return animationFrame;
}

void StairsItem::setAnimationFrame(unsigned int animationFrame)
{
	animationFrame = (animationFrame % 14);
	if (this->animationFrame != animationFrame) {
		this->animationFrame = animationFrame;
		updateBackground();
	}
}

double StairsItem::getAnimationProgress()
{
	return animationProgress;
}

void StairsItem::setAnimationProgress(double animationProgress)
{
	this->animationProgress = animationProgress;
	setAnimationFrame(animationProgress * 13 + 1);
}

bool StairsItem::isInUse()
{
	return inUse;
}

void StairsItem::setInUse(bool inUse)
{
	if (this->inUse != inUse) {
		this->inUse = inUse;
		if (inUse)
			setAnimationProgress(0.0);
		else
			setAnimationFrame(0);
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Basic Sprites
//----------------------------------------------------------------------------------------------------

void StairsItem::initBackground()
{
	Item::initBackground();
	
	//Setup the background texture rects
	for (int i = 0; i < 2; i++)
		backgrounds[i].textureRect.size = double2(1.0 / 7, 1);
}

void StairsItem::updateBackground()
{
	Item::updateBackground();
	
	//Update the background textures
	for (int i = 0; i < 2; i++) {
		char textureName[128];
		sprintf(textureName, "simtower/transport/stairs/floor%i/%i", i, getAnimationFrame() / 7);
		backgrounds[i].texture = Texture::named(textureName);
		backgrounds[i].textureRect.origin.x = (getAnimationFrame() % 7) / 7.0;
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

void StairsItem::advance(double dt)
{
	Item::advance(dt);
	
	//Animate the stairs if in use
	if (isInUse())
		setAnimationProgress(fmod(getAnimationProgress() + 1.5 * dt, 1.0));
}
