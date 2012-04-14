#include "../Sprite.h"
#include "Elevator.h"

using namespace OT::Item;


void Elevator::init()
{
	layer = 1;
	
	Item::init();
	
	animation = 0;
	frame = 0;
	
	shaft.SetImage(app->bitmaps[shaftBitmap]);
	shaft.SetSubRect(sf::IntRect(0, 0, size.x*8, 36));
	shaft.SetCenter(0, 36);
	
	topMotor.SetImage(*shaft.GetImage());
	bottomMotor.SetImage(*shaft.GetImage());
	
	addSprite(&topMotor);
	addSprite(&bottomMotor);
	
	updateSprite();
}

void Elevator::updateSprite()
{
	int w = GetSize().x;
	int h = GetSize().y;
	
	topMotor.SetSubRect   (sf::IntRect((2*frame+1)*w, 0, (2*frame+2)*w, 36));
	bottomMotor.SetSubRect(sf::IntRect((2*frame+2)*w, 0, (2*frame+3)*w, 36));
	topMotor.SetCenter(0, 36);
	topMotor.SetY(-h);
}

void Elevator::Render(sf::RenderTarget & target) const
{
	Item::Render(target);
	
	Sprite s = shaft;
	Sprite d;
	d.SetImage(app->bitmaps["simtower/elevator/digits"]);
	d.SetCenter(0, 17);
	
	int minY = 0;
	int maxY = size.y-1;
	
	for (int y = minY; y <= maxY; y++) {
		s.SetY(-y*36);
		d.SetY(-y*36 - 3);
		target.Draw(s);
		
		char c[8];
		int len = snprintf(c, 8, "%i", position.y + y);
		int x = 11 - (len - 1) * 6;
		for (int i = 0; i < len; i++) {
			int p = 10;
			if (c[i] >= '0' && c[i] <= '9') p = c[i] - '0';
			d.SetSubRect(sf::IntRect(p*11, 0, (p+1)*11, 17));
			d.SetX(x);
			target.Draw(d);
			x += 12;
		}
	}
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

void Elevator::encodeXML(tinyxml2::XMLPrinter & xml)
{
	Item::encodeXML(xml);
	xml.PushAttribute("height", size.y);
}

void Elevator::decodeXML(tinyxml2::XMLElement & xml)
{
	Item::decodeXML(xml);
	size.y = xml.IntAttribute("height");
	updateSprite();
}
