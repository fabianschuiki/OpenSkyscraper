#include "Stairlike.h"

using namespace OT::Item;


void Stairlike::init()
{
	Item::init();
	
	animation = 0;
	frame = 0;
	layer = 1;
	
	addSprite(&sprite);
	
	updateSprite();
}

void Stairlike::advance(double dt)
{
	Item::advance(dt);
	
	animation = fmod(animation + dt*1.5, 1);
	int newFrame = floor(animation * (frameCount-1))+1;
	if (frame != newFrame) {
		frame = newFrame;
		updateSprite();
	}
}

void Stairlike::updateSprite()
{
	int w = sprite.GetImage()->GetWidth() / frameCount;
	int h = sprite.GetImage()->GetHeight();
	
	sprite.SetCenter(0, h);
	sprite.SetSubRect(sf::IntRect(w*frame, 0, w*(frame+1), h));
	sprite.Resize(w, h);
}
