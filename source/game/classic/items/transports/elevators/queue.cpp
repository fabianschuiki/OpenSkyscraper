#include "queue.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

ElevatorQueue::ElevatorQueue(ElevatorItem * elevator) : elevator(elevator),
updatePersonSpritesIfNeeded(this, &ElevatorQueue::updatePersonSprites, &updateIfNeeded),
updateCallSpriteIfNeeded(this, &ElevatorQueue::updateCallSprite, &updateIfNeeded)
{
	called = false;
	callTime = 0;
	steppingInside = false;
	
	//Initialize the call sprite
	callSprite = new Sprite;
	callSprite->rect.size = double2(8, 8);
	callSprite->texture = Texture::named("transport/elevator/calls.png");
	callSprite->textureRect.size = double2(0.5, 0.5);
}

string ElevatorQueue::className() const
{
	return "queue";
}

string ElevatorQueue::instanceName() const
{
	stringstream s;
	s << className();
	s << " of ";
	s << elevator->description();
	s << " on floor ";
	s << getRect().minY();
	s << " ";
	s.setf(std::ios::hex);
	s << this;
	return s.str();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Layout
//----------------------------------------------------------------------------------------------------

const recti & ElevatorQueue::getRect() const
{
	return rect;
}

void ElevatorQueue::setRect(const recti & r)
{
	if (rect != r) {
		rect = r;
		updatePersonSpritesIfNeeded.setNeeded();
	}
}

rectd ElevatorQueue::getWorldRect() const
{
	return elevator->tower->structure->cellToWorld(getRect());
}

ElevatorItem::Direction ElevatorQueue::getDirection()
{
	return direction;
}

void ElevatorQueue::setDirection(ElevatorItem::Direction dir)
{
	if (direction != dir) {
		direction = dir;
		updatePersonSpritesIfNeeded.setNeeded();
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark People
//----------------------------------------------------------------------------------------------------

void ElevatorQueue::addPerson(Person * p)
{
	people.push_back(p);
	updatePersonSpritesIfNeeded.setNeeded();
	callCar();
}

void ElevatorQueue::removePerson(Person * p)
{
	people.remove(p);
	personSprites.erase(p);
}

bool ElevatorQueue::hasPeople()
{
	return !people.empty();
}

Person * ElevatorQueue::popPerson()
{
	if (people.empty())
		return NULL;
	Person * p = people.front();
	people.pop_front();
	personSprites.erase(p);
	updatePersonSpritesIfNeeded.setNeeded();
	return p;
}

bool ElevatorQueue::isSteppingInside()
{
	return steppingInside;
}

void ElevatorQueue::setSteppingInside(bool si)
{
	if (steppingInside != si) {
		steppingInside = si;
		updatePersonSpritesIfNeeded.setNeeded();
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Response
//----------------------------------------------------------------------------------------------------

bool ElevatorQueue::isCalled()
{
	return called;
}

double ElevatorQueue::getWaitDuration()
{
	return (elevator->tower->time->getTime() - callTime);
}

ElevatorCar * ElevatorQueue::getRespondingCar()
{
	return respondingCar;
}

void ElevatorQueue::answerCall(ElevatorCar * car)
{
	assert(!respondingCar);
	respondingCar = car;
	updateCallSpriteIfNeeded.setNeeded();
}

bool ElevatorQueue::isCallAnswered()
{
	return (called && respondingCar);
}

void ElevatorQueue::callCar()
{
	if (!called) {
		called = true;
		callTime = elevator->tower->time->getTime();
		elevator->respondToCallsIfNeeded.setNeeded();
		updateCallSpriteIfNeeded.setNeeded();
	}
}

void ElevatorQueue::clearCall()
{
	called = false;
	callTime = 0;
	respondingCar = NULL;
	
	//If there are still people queueing up, reissue a call
	if (hasPeople())
		callCar();
	
	//Update the call sprite
	updateCallSpriteIfNeeded.setNeeded();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark State
//----------------------------------------------------------------------------------------------------

void ElevatorQueue::update()
{
	updatePersonSpritesIfNeeded();
	//updateCallSpriteIfNeeded();
}

void ElevatorQueue::updatePersonSprites()
{	
	//We need a running variable for the x coordinate
	double x = 16;
	
	//For each person in the queue we have to update the sprite that is used to represent him/her in
	//the queue.
	for (PersonList::iterator it = people.begin(); it != people.end(); it++) {
		
		//Make working with the person easier
		Person * person = (*it);
		
		//Decide whether this person should be drawn stepping inside the elevator.
		bool stepping = (isSteppingInside() && people.front() == person);
		
		//If the person is stepping inside we may move the x coordinate back by 16 pixels. This is
		//because the queue is formed starting 16 pixels away from the elevator. The stepping ani-
		//mations are designed to be drawn touching the elevator which would be 0 pixels. And since
		//we start at x = 16 we may move it back by 16 to start at 0.
		if (stepping)
			x -= 16;
		
		//Create the sprite if required
		if (!personSprites[person])
			personSprites[person] = new Sprite;
		
		//Choose the appropriate texture and general texture area.
		personSprites[person]->texture = person->getQueueingTexture();
		
		//Get the overall texture rect so we know where to look for our animation frames in the
		//texture.
		rectd texrect = person->getQueueingTextureRect();
		
		//Calculate the subrect we need.
		rectd subtexrect(0, 0, 1.0 / 10 * 2, 1);
		subtexrect.origin.x = 1.0 / 10 * (stepping ? 3 : 0);
		
		//If the person is queueing for the up direction, we have to make the texture subrect align-
		//ed at the right end of the texture rect instead of the left end.
		if (direction == ElevatorItem::kUp)
			subtexrect.origin.x = 1.0 - subtexrect.origin.x - subtexrect.size.x;
		
		//Transform the subtexrect which is in texrect coordinate space into the coordinate space of
		//the whole texture.
		subtexrect.origin *= texrect.size;
		subtexrect.size *= texrect.size;
		subtexrect.origin += texrect.origin;
		
		//Set the texture subrect
		personSprites[person]->textureRect = subtexrect;
		
		
		//Calculate the person rect
		rectd rect(0, getWorldRect().minY(), 16, 36);
		if (direction == ElevatorItem::kDown)
			rect.origin.x = (getWorldRect().minX() + x);
		else
			rect.origin.x = (getWorldRect().maxX() - rect.size.x - x);
		
		//Apply the rect we just calculated.
		personSprites[person]->rect = rect;
		
		//Advance the running variable by either 8 or 16 pixels, depending on the space usage of the
		//person type. We have to differentiate between a person stepping in and one that doesn't.
		//In any case, a person stepping in doesn't require any room since it's animation starts
		//right at the elevator. So if this is a normal person, we simply increase x. But if this is
		//a person stepping in, we increase x by 16 again since this is the amount by which we re-
		//duced the value at the beginning of the loop iteration. This will make people after the
		//one person stepping inside queueing up normally at 16 pixels.
		if (stepping)
			x += 16;
		else
			x += person->getQueueingWidth();
		
		//If we exceeded the queue width, break out of the loop and avoid creating any other
		//sprites.
		if (x >= getWorldRect().size.x - 8)
			break;
	}
}

void ElevatorQueue::updateCallSprite()
{
	//Reposition the call sprite vertically.
	callSprite->rect.origin.y = getWorldRect().minY() + 23;
	
	//Depending on the direction we have to choose different horizontal positions and texture rects.
	if (direction == ElevatorItem::kDown) {
		callSprite->rect.origin.x = getWorldRect().minX() - 4 - callSprite->rect.size.x;
		callSprite->textureRect.origin.x = 0;
	} else {
		callSprite->rect.origin.x = getWorldRect().maxX() + 4 + 6;
		callSprite->textureRect.origin.x = 0.5;
	}
	
	//Depending on whether the call is answered or not we have to choose a different portion of the
	//texture rect.
	callSprite->textureRect.origin.y = (isCallAnswered() ? 0 : 0.5);
	
	//If we aren't called at all, we may simply hide the sprite.
	callSprite->hidden = !isCalled();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

void ElevatorQueue::draw(rectd dirtyRect)
{
	//Draw the person sprites
	for (PersonSpriteMap::iterator it = personSprites.begin(); it != personSprites.end(); it++)
		it->second->draw();
	
	//Draw the call sprite
	//callSprite->draw();
}
