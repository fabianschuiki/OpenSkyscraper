#include "elevator.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

ElevatorItem::ElevatorItem(Tower * tower, ItemDescriptor * descriptor)
: TransportItem(tower, descriptor), background(this)
{
	motorbufferAnimationIndex = 0;
	motorbufferAnimation = 0;
	animateMotorBuffer = true;
	hasUnionBackground = true;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Basic Sprites
//----------------------------------------------------------------------------------------------------

void ElevatorItem::initBasicSprites()
{
	TransportItem::initBasicSprites();
	initMotorBuffer();
}

void ElevatorItem::updateBasicSprites()
{
	TransportItem::updateBasicSprites();
	updateMotorBuffer();
	background.update();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Background
//----------------------------------------------------------------------------------------------------

void ElevatorItem::initBackground()
{
	TransportItem::initBackground();
	backgrounds[0].texture = &background;
}

bool ElevatorItem::isFloorActive(int floor)
{
	return true;
}

bool ElevatorItem::isFloorHighlighted(int floor)
{
	return (floor == 0);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Motor and Buffer
//----------------------------------------------------------------------------------------------------

void ElevatorItem::initMotorBuffer()
{
	//The motor is the left, the buffer the right half of the texture
	motorSprite.textureRect = rectd(0, 0, 0.5, 1);
	bufferSprite.textureRect = rectd(0.5, 0, 0.5, 1);
}

void ElevatorItem::updateMotorBuffer()
{
	//Calculate the rect for the motor sprite
	recti motorRect = getRect();
	motorRect.origin.y = motorRect.maxY();
	motorRect.size.y = 1;
	
	//Calculate the rect for the buffer sprite
	recti bufferRect = motorRect;
	bufferRect.origin.y = getRect().minY() - bufferRect.size.y;
	
	//Position the motor and buffer
	motorSprite.setRect(tower->convertCellToWorldRect(motorRect));
	bufferSprite.setRect(tower->convertCellToWorldRect(bufferRect));
	
	//Load the appropriate textures
	char textureName[512];
	sprintf(textureName, "simtower/transport/elevator/standard/motorbuffer/%i",
			getMotorbufferAnimationIndex());
	motorSprite.texture = Engine::Texture::named(textureName);
	bufferSprite.texture = motorSprite.texture;
}



bool ElevatorItem::getAnimateMotorBuffer()
{
	return animateMotorBuffer;
}

void ElevatorItem::setAnimateMotorBuffer(bool animate)
{
	if (this->animateMotorBuffer != animate) {
		this->animateMotorBuffer = animate;
		if (!animate)
			setMotorbufferAnimation(0);
	}
}

unsigned int ElevatorItem::getMotorbufferAnimationIndex()
{
	return motorbufferAnimationIndex;
}

void ElevatorItem::setMotorbufferAnimationIndex(unsigned int index)
{
	if (motorbufferAnimationIndex != index) {
		motorbufferAnimationIndex = index;
		updateMotorBuffer();
	}
}

double ElevatorItem::getMotorBufferAnimation()
{
	return motorbufferAnimation;
}

void ElevatorItem::setMotorbufferAnimation(double time)
{
	if (this->motorbufferAnimation != time) {
		this->motorbufferAnimation = time;
		setMotorbufferAnimationIndex(3 * time);
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

void ElevatorItem::advance(double dt)
{
	TransportItem::advance(dt);
	
	//Advance the motor and buffer animation
	if (getAnimateMotorBuffer())
		advanceMotorBuffer(dt);
}

void ElevatorItem::advanceMotorBuffer(double dt)
{
	double time = getMotorBufferAnimation();
	time += dt;
	setMotorbufferAnimation(fmod(time, 1));
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

void ElevatorItem::draw(rectd visibleRect)
{
	TransportItem::draw(visibleRect);
	
	//Draw the motor and buffer
	motorSprite.draw(visibleRect);
	bufferSprite.draw(visibleRect);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Notifications
//----------------------------------------------------------------------------------------------------

void ElevatorItem::onChangeLocation()
{
	background.setNeedsUpdate();
	TransportItem::onChangeLocation();
}
