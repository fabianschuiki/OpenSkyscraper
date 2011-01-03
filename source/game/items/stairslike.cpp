#include "stairslike.h"
#include "../tower.h"

using namespace OSS;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

StairslikeItem::StairslikeItem(Tower * tower, Descriptor * descriptor) : TransportItem(tower, descriptor)
{
	animationFrame = 0;
	animationProgress = 0;
	inUse = false;
	numAnimationFrames = 0;
	numAnimationFramesPerTexture = 0;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Connection
//----------------------------------------------------------------------------------------------------

std::set<int> StairslikeItem::getConnectionFloors()
{
	std::set<int> floors;
	floors.insert(getMinFloor());
	floors.insert(getMaxFloor());
	return floors;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Animation
//----------------------------------------------------------------------------------------------------

unsigned int StairslikeItem::getAnimationFrame()
{
	return animationFrame;
}

void StairslikeItem::setAnimationFrame(unsigned int animationFrame)
{
	animationFrame = (animationFrame % numAnimationFrames);
	if (this->animationFrame != animationFrame) {
		this->animationFrame = animationFrame;
		updateBackground();
	}
}

double StairslikeItem::getAnimationProgress()
{
	return animationProgress;
}

void StairslikeItem::setAnimationProgress(double animationProgress)
{
	this->animationProgress = animationProgress;
	setAnimationFrame(animationProgress * (numAnimationFrames - 1) + 1);
}

bool StairslikeItem::isInUse()
{
	return inUse;
}

void StairslikeItem::setInUse(bool inUse)
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

void StairslikeItem::initBackground()
{
	TransportItem::initBackground();
	
	//Setup the background texture rects
	for (int i = 0; i < 2; i++)
		backgrounds[i].textureRect.size = double2(1.0 / numAnimationFramesPerTexture, 1);
}

void StairslikeItem::updateBackground()
{
	TransportItem::updateBackground();
	
	//Update the background textures
	for (int i = 0; i < 2; i++) {
		//Assemble the texture name
		char textureName[128];
		sprintf(textureName, "/floor%i", i);
		if (numAnimationFrames > numAnimationFramesPerTexture)
			sprintf(textureName, "%s/%i", textureName, getAnimationFrame() / numAnimationFramesPerTexture);
		backgrounds[i].texture = Texture::named(baseTextureName + textureName);
		
		//Calculate the texture x coordinate
		double x = (getAnimationFrame() % numAnimationFramesPerTexture);
		x /= numAnimationFramesPerTexture;
		backgrounds[i].textureRect.origin.x = x;
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

void StairslikeItem::advance(double dt)
{
	TransportItem::advance(dt);
	
	//Animate the stairs if in use
	if (isInUse())
		setAnimationProgress(fmod(getAnimationProgress() + 1.5 * dt, 1.0));
}

