#include "elevator.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

ElevatorItem::ElevatorItem(Tower * tower, ItemDescriptor * descriptor)
: TransportItem(tower, descriptor)
{
	animationFrame = 0;
	
	//Load the required textures
	backgroundNormal		= Texture::named("simtower/transport/floordigits/one/normal");
	backgroundHighlighted	= Texture::named("simtower/transport/floordigits/one/highlighted");
	lsNormal				= Texture::named("simtower/transport/floordigits/two/ls/normal");
	lsHighlighted			= Texture::named("simtower/transport/floordigits/two/ls/highlighted");
	msNormal				= Texture::named("simtower/transport/floordigits/two/ms/normal");
	msHighlighted			= Texture::named("simtower/transport/floordigits/two/ms/highlighted");
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Layout
//----------------------------------------------------------------------------------------------------

bool ElevatorItem::isFloorActive(int floor)
{
	return true;
}

bool ElevatorItem::isFloorHighlighted(int floor)
{
	return (floor == 0);
}

recti ElevatorItem::getMotorRect()
{
	recti r = getRect();
	r.origin.y = r.maxY();
	r.size.y = 1;
	return r;
}

recti ElevatorItem::getBufferRect()
{
	recti r = getRect();
	r.origin.y = r.minY() - 1;
	r.size.y = 1;
	return r;
}

recti ElevatorItem::getCarsRect()
{
	return getRect();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------


unsigned int ElevatorItem::getAnimationFrame()
{
	return animationFrame;
}

void ElevatorItem::setAnimationFrame(unsigned int af)
{
	if (animationFrame != af) {
		animationFrame = af;
		updateBackgroundIfNeeded.setNeeded();
	}
}

void ElevatorItem::advanceItem(double dt)
{
	TransportItem::advanceItem(dt);
	
	//Advance the animation
	if (shouldAnimate() && shouldAdvance("animation", 1.0 / 3))
		setAnimationFrame((getAnimationFrame() + 1) % 3);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark State
//----------------------------------------------------------------------------------------------------

void ElevatorItem::updateBackground()
{
	//Note that we don't call the superclass's updateBackground since that would initialize the
	//background sprites which we don't want for the elevator since we're drawing the complicated
	//background ourselves without sprites.
	
	//Initialize the motor and buffer sprites if required
	if (!motorSprite)
		motorSprite = new Sprite;
	if (!bufferSprite)
		bufferSprite = new Sprite;
	
	//The motor is the left, the buffer the right half of the texture
	motorSprite->textureRect = rectd(0, 0, 0.5, 1);
	bufferSprite->textureRect = rectd(0.5, 0, 0.5, 1);
	
	//Position the motor and buffer
	motorSprite->rect = (tower->structure->cellToWorld(getMotorRect()));
	bufferSprite->rect = (tower->structure->cellToWorld(getBufferRect()));
	
	//Load the appropriate textures
	stringstream textureName;
	textureName << "simtower/transport/elevator/";
	textureName << getMotorBufferTypeName();
	textureName << "/motorbuffer/";
	textureName << getAnimationFrame();
	motorSprite->texture = Texture::named(textureName.str());
	bufferSprite->texture = motorSprite->texture;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

void ElevatorItem::drawItem(rectd dirtyRect)
{
	TransportItem::drawItem(dirtyRect);
	
	//Draw the elevator cars
	drawCars(dirtyRect);
}

void ElevatorItem::drawBackground(rectd dirtyRect)
{
	TransportItem::drawBackground(dirtyRect);
	
	//Draw the motor and buffer
	motorSprite->draw();
	bufferSprite->draw();
	
	//Decide the lowest and highest visible floors.
	recti dirtyCells = tower->structure->worldToCell(dirtyRect);
	recti visibleCells = dirtyCells.intersectionRect(getRect());
	int lowestFloor = visibleCells.minY();
	int highestFloor = visibleCells.maxY() - 1;
	
	//Draw each visible floor.
	for (int i = lowestFloor; i <= highestFloor; i++)
		drawFloor(i);
}

void ElevatorItem::drawFloor(int f)
{
	//Calculate this floor's rect
	rectd rect = getFloorWorldRect(f);
	
	//Draw the floor background
	drawFloorBackground(f, rect);
	
	//Draw the floor number if the floor is active.
	if (isFloorActive(f)) {
		rectd numberRect = rect;
		numberRect.origin.x += 2;
		numberRect.size.x -= 2;
		drawFloorNumber(f, numberRect);
	}
}

void ElevatorItem::drawFloorBackground(int f, rectd rect)
{
	//Create a textured quad which we will use for drawing the background. We may also load the
	//background texture right at the beginning, since the different drawing steps all use areas
	//of the same background texture.
	TexturedQuad quad;
	quad.texture = backgroundNormal;
	
	//Draw the left wall.
	quad.rect = rect;
	quad.rect.size.x = 16;
	quad.textureRect.size.x = 16 / 352.0;
	quad.draw();
	
	//Draw the middle part
	quad.rect = rect.insetRect(int2(16, 0));
	quad.textureRect.origin.x = 10 / 352.0;
	quad.draw();
	
	//Draw the right wall
	quad.rect = rect;
	quad.rect.origin.x = quad.rect.maxX() - 16;
	quad.rect.size.x = 16;
	quad.textureRect.origin.x = quad.textureRect.size.x;
	quad.draw();
}

void ElevatorItem::drawFloorNumber(int f, rectd rect)
{
	//Create the textured quad we'll use for drawing the floor numbers.
	TexturedQuad quad;
	quad.rect = rect;
	
	//Decide what textures we're supposed to use
	bool highlighted = isFloorHighlighted(f);
	Texture * background = (highlighted ? backgroundHighlighted : backgroundNormal);
	Texture * ls = (highlighted ? lsHighlighted : lsNormal);
	Texture * ms = (highlighted ? msHighlighted : msNormal);
	
	//100th floor has its own bitmap
	if (f == 100) {
		quad.rect.size.x = 32;
		quad.rect.origin.x = rect.midX() - (quad.rect.size.x / 2);
		quad.texture = background;
		quad.textureRect = rectd(10.0 / 11, 0, 1.0 / 11, 1);
		quad.draw();
		return;
	}
	
	//Check whether the floor is negative
	bool negative = (f < 0);
	
	//Extract the MSD and LSD
	unsigned int msd = abs(f) / 10;
	unsigned int lsd = abs(f) % 10;
	
	//Calculate how many digits there will be
	unsigned int numberOfDigits = 1;
	if (msd > 0) numberOfDigits++;
	if (negative) numberOfDigits++;
	
	//Calculate the origin x coordinate
	quad.rect.origin.x = rect.midX() - (numberOfDigits * 12 / 2.0);
	
	//All digits share the same rect width
	quad.rect.size.x = 13;
	
	//Draw the B for negative floors if required
	if (negative) {
		//We need the ms texture for this
		quad.texture = ms;
		
		//Choose the texture rect which contains the B letter
		quad.textureRect.origin.x = 35.0 / ms->size.x;
		quad.textureRect.size.x = 13.0 / ms->size.x;
		
		//Draw the quad and advance its x coordinate for the next digit.
		quad.draw();
		quad.rect.origin.x += 12;
	}
	
	//The LS digits share the same texture and texture rect width
	quad.texture = ls;
	quad.textureRect.size.x = 13.0 / ls->size.x;
	
	//Draw the most significant digit, if there is one
	if (msd > 0) {
		//Choose the appropriate texture rect based on the digit value.
		quad.textureRect.origin.x = msd / 10.0;
		
		//Draw the quad and advance
		quad.draw();
		quad.rect.origin.x += 12;
		
		//Do some visual improvement. If the most significant digit was a 1, we may move the lsd a
		//bit closer.
		if (msd == 1)
			quad.rect.origin.x -= 2;
	}
	
	//Draw the least significant digit
	quad.textureRect.origin.x = lsd / 10.0;
	quad.draw();
}

void ElevatorItem::drawCars(rectd dirtyRect)
{
}
