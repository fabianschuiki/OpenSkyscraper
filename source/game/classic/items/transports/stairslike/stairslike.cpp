#include "stairslike.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

StairslikeItem::StairslikeItem(Tower * tower, ItemDescriptor * descriptor) : TransportItem(tower, descriptor)
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
#pragma mark State
//----------------------------------------------------------------------------------------------------

void StairslikeItem::updateBackground()
{
	TransportItem::updateBackground();
	
	//Update the background textures
	for (int i = 0; i < 2; i++) {
		//Assemble the texture name
		stringstream textureName;
		textureName << "/floor" << i;
		if (numAnimationFrames > numAnimationFramesPerTexture)
			textureName << "/" << (getAnimationFrame() / numAnimationFramesPerTexture);
		backgrounds[i]->texture = Texture::named(baseTextureName + textureName.str());
		backgrounds[i]->textureRect.size = double2(1.0 / numAnimationFramesPerTexture, 1);
		
		//Calculate the texture x coordinate
		double x = (getAnimationFrame() % numAnimationFramesPerTexture);
		x /= numAnimationFramesPerTexture;
		backgrounds[i]->textureRect.origin.x = x;
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

void StairslikeItem::advanceItem(double dt)
{
	TransportItem::advanceItem(dt);
	
	//Advance people's transit
	People peopleToAdvance;
	for (TransitProgressMap::iterator p = transitProgress.begin(); p != transitProgress.end(); p++) {
		if (p->second >= 1) 
			continue;
		p->second += dt / 1.0;
		if (p->second >= 1)
			peopleToAdvance.insert(p->first);
	}
	
	//Advance people's journey where transit is over
	for (People::iterator p = peopleToAdvance.begin(); p != peopleToAdvance.end(); p++) {
		(*p)->setFloor((*p)->getEndFloor());
		//(*p)->advanceJourney();
	}
	
	//Animate the stairs if in use
	if (isInUse())
		setAnimationProgress(fmod(getAnimationProgress() + 1.5 * dt, 1.0));
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark People
//----------------------------------------------------------------------------------------------------

void StairslikeItem::didAddPerson(Person * person)
{
	//Create a transit progress entry for this person
	transitProgress[person] = 0.0;
	
	//Animate the item
	setInUse(true);
}

void StairslikeItem::willRemovePerson(Person * person)
{
	//Get rid of the transit progress entry
	transitProgress.erase(person);
	
	//Stop the animation if there are no more people using this item
	if (transitProgress.empty())
		setInUse(false);
}
