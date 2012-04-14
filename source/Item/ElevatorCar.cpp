#include "ElevatorCar.h"

using namespace OT::Item;


void ElevatorCar::init()
{
	sprite.SetImage(app->bitmaps[elevator->carBitmap]);
	sprite.SetCenter(0, 30);
	updateSprite();
}

void ElevatorCar::setAltitude(double a)
{
	if (altitude != a) {
		altitude = a;
		reposition();
	}
}

void ElevatorCar::reposition()
{
	SetPosition(0, -altitude * 36 - elevator->GetPosition().y);
}

void ElevatorCar::updateSprite()
{
	int state = 3;
	int pc = passengers.size();
	if (pc == 0) state = 0;
	else if (pc <= 1) state = 1;
	else if (pc <= 3) state = 2;
	else if (pc == elevator->maxPassengers) state = 4;
	
	int w = sprite.GetImage()->GetWidth() / 5;
	int h = sprite.GetImage()->GetHeight();
	sprite.SetSubRect(sf::IntRect(state*w, 0, (state+1)*w, h));
	sprite.SetPosition(sf::Vector2f(2, 0));
}

void ElevatorCar::Render(sf::RenderTarget & target) const
{
	target.Draw(sprite);
}

void ElevatorCar::encodeXML(tinyxml2::XMLPrinter& xml)
{
	xml.PushAttribute("altitude", altitude);
}

void ElevatorCar::decodeXML(tinyxml2::XMLElement& xml)
{
	setAltitude(xml.DoubleAttribute("altitude"));
}
