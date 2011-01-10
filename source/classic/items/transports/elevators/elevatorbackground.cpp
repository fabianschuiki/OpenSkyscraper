#include "elevatorbackground.h"

#include "elevator.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

ElevatorBackground::ElevatorBackground(ElevatorItem * elevator)
: Engine::Texture("elevatorBackground"), elevator(elevator)
{
	assert(elevator);
	needsUpdate = false;
	
	//Fetch the textures
	backgroundNormal		= Engine::Texture::named("simtower/transport/floordigits/one/normal");
	backgroundHighlighted	= Engine::Texture::named("simtower/transport/floordigits/one/highlighted");
	lsNormal				= Engine::Texture::named("simtower/transport/floordigits/two/ls/normal");
	lsHighlighted			= Engine::Texture::named("simtower/transport/floordigits/two/ls/highlighted");
	msNormal				= Engine::Texture::named("simtower/transport/floordigits/two/ms/normal");
	msHighlighted			= Engine::Texture::named("simtower/transport/floordigits/two/ms/highlighted");
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Update
//----------------------------------------------------------------------------------------------------

void ElevatorBackground::setNeedsUpdate()
{
	needsUpdate = true;
}

void ElevatorBackground::update()
{
	if (!needsUpdate) return;
	
	//Make sure we have all required textures
	if (!backgroundNormal->isLoaded) return;
	if (!backgroundHighlighted->isLoaded) return;
	if (!lsNormal->isLoaded) return;
	if (!lsHighlighted->isLoaded) return;
	if (!msNormal->isLoaded) return;
	if (!msHighlighted->isLoaded) return;
	
	//Calculate the image size
	int2 size = elevator->getWorldRect().size;
	
	//Get the number of floors
	unsigned int numFloors = elevator->getNumFloors();
	
	//Calculate the floor size
	int2 floorSize(size.x, size.y / numFloors);
	
	//Create the new image
	ILuint image = ilGenImage();
	ilBindImage(image);
	
	//Set the image's dimensions
	ilTexImage(size.x, size.y, 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, NULL);
	ilClearColour(1, 0, 0, 1);
	ilClearImage();
	
	//Draw the floors
	for (int i = 0; i < numFloors; i++) {
		//Calculate the floor rect
		rectd floorRect(int2(0, floorSize.y * i), floorSize);
		
		//Draw the floor
		drawFloor(elevator->getMaxFloor() - i, floorRect);
	}
	
	ilSaveImage("/Users/fabian/Desktop/background.png");
	
	//Assign, load and refinalize
	assignLoadedImage(image);
	load();
	finalize();
	
	//We don't need to update anymore
	needsUpdate = false;
}

void ElevatorBackground::drawFloor(int floor, rectd rect)
{
	//Draw the background
	drawFloorBackground(rect);
	
	//Only proceed with the labelling if the floor is actually active
	if (elevator->isFloorActive(floor)) {
		//Calculate the number rect
		rectd numberRect = rect;
		numberRect.origin.x += 2;
		numberRect.size.x -= 2;
		
		//Draw the number
		drawFloorNumber(floor, numberRect);
	}
}

void ElevatorBackground::drawFloorBackground(rectd rect)
{
	//Draw the left wall
	ilBlit(backgroundNormal->tempImage,
		   rect.minX(), rect.minY(), 0,
		   0, 0, 0,
		   16, 36, 1);
	
	//Fill in the background
	for (double x = rect.minX() + 16; x < rect.maxX() - 16; x += 16)
		ilBlit(backgroundNormal->tempImage,
			   x, rect.minY(), 0,
			   10, 0, 0,					//10 = width of the left wall
			   16, 36, 1);
	
	//Draw the right end
	ilBlit(backgroundNormal->tempImage,
		   rect.maxX() - 16, rect.minY(), 0,
		   16, 0, 0,
		   16, 36, 1);
}

void ElevatorBackground::drawFloorNumber(int floor, rectd rect)
{
	//Decide what textures we're supposed to use
	bool highlighted = elevator->isFloorHighlighted(floor);
	Texture * background = (highlighted ? backgroundHighlighted : backgroundNormal);
	Texture * ls = (highlighted ? lsHighlighted : lsNormal);
	Texture * ms = (highlighted ? msHighlighted : msNormal);
	
	//100th floor has its own bitmap
	if (floor == 100) {
		unsigned int x = rect.midX() - (23 / 2.0);
		ilBlit(background->tempImage,
			   x, rect.minY(), 0,
			   324, 0, 0,
			   23, 36, 1);
		return;
	}
	
	//Check whether the floor is negative
	bool negative = (floor < 0);
	
	//Extract the MSD and LSD
	unsigned int msd = abs(floor) / 10;
	unsigned int lsd = abs(floor) % 10;
	
	//Calculate how many digits there will be
	unsigned int numberOfDigits = 1;
	if (msd > 0) numberOfDigits++;
	if (negative) numberOfDigits++;
	
	//Calculate the origin x coordinate
	unsigned int x = rect.midX() - (numberOfDigits * 12 / 2.0);
	
	//Draw the B for negative floors if required
	if (negative) {
		ilBlit(ms->tempImage,
			   x, rect.minY(), 0,
			   35, 0, 0,
			   13, 36, 1);
		x += 12;
	}
	
	//Draw the most significant digit, if there is one
	if (msd > 0) {
		ilBlit(ls->tempImage,
			   x, rect.minY(), 0,
			   msd * 16, 0, 0,
			   13, 36, 1);
		x += 12;
		if (msd == 1)
			x -= 2;
	}
	
	//Draw the least significant digit
	ilBlit(ls->tempImage,
		   x, rect.minY(), 0,
		   lsd * 16, 0, 0,
		   13, 36, 1);
}
