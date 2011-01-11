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
		backgrounds[i].texture = Engine::Texture::named(baseTextureName + textureName);
		
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
		(*p)->setFloor((*p)->getNextFloor());
		(*p)->advanceJourney();
	}
	
	//Animate the stairs if in use
	if (isInUse())
		setAnimationProgress(fmod(getAnimationProgress() + 1.5 * dt, 1.0));
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark People
//----------------------------------------------------------------------------------------------------

void StairslikeItem::addPerson(Person * person)
{
	TransportItem::addPerson(person);
	
	//Create a transit progress entry for this person
	transitProgress[person] = 0.0;
	
	//Animate the item
	setInUse(true);
}

void StairslikeItem::removePerson(Person * person)
{
	//Get rid of the transit progress entry
	transitProgress.erase(person);
	
	//Actually remove the person
	TransportItem::removePerson(person);
	
	//Stop the animation if there are no more people using this item
	if (people.empty())
		setInUse(false);
}
