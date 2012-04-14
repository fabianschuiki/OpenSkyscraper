#include "Elevator.h"

using namespace OT::Item;


void Elevator::init()
{
	animation = 0;
	frame = 0;
	
	Item::init();
	
	shaft.SetImage(app->bitmaps[shaftBitmap]);
	topMotor.SetImage(*shaft.GetImage());
	bottomMotor.SetImage(*shaft.GetImage());
	
	addSprite(&shaft);
	addSprite(&topMotor);
	addSprite(&bottomMotor);
	
	updateSprite();
}

void Elevator::updateSprite()
{
	int w = GetSize().x;
	int h = GetSize().y;
	
	shaft.SetSubRect(sf::IntRect(0, 0, w, h));
	shaft.Resize(w, h);
	shaft.SetCenter(0, h);
	
	topMotor.SetSubRect   (sf::IntRect((2*frame+1)*w, 0, (2*frame+2)*w, h));
	bottomMotor.SetSubRect(sf::IntRect((2*frame+2)*w, 0, (2*frame+3)*w, h));
	topMotor.SetCenter(0, 36);
	topMotor.SetY(-h);
}

void Elevator::advance(double dt)
{
	bool carsMoving = true; //TODO: actually check for moving cars.
	if (carsMoving) {
		animation = fmod(animation + dt, 1);
		int newFrame = floor(animation * 3);
		if (frame != newFrame) {
			frame = newFrame;
			updateSprite();
		}
	} else {
		animation = 0;
		if (frame != 0) {
			frame = 0;
			updateSprite();
		}
	}
}
