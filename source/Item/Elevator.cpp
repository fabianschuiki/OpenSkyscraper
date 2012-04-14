#include <cassert>
#include "../Sprite.h"
#include "Elevator.h"
#include "ElevatorCar.h"

using namespace OT::Item;
using OT::rectd;


Elevator::~Elevator()
{
	clearCars();
}

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
	
	addCar(position.y);
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
		
		int flr = position.y + y;
		if (unservicedFloors.count(flr)) continue;
		
		char c[8];
		int len = snprintf(c, 8, "%i", flr);
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
	
	for (Cars::iterator c = cars.begin(); c != cars.end(); c++) {
		target.Draw(**c);
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
	for (std::set<int>::iterator i = unservicedFloors.begin(); i != unservicedFloors.end(); i++) {
		xml.OpenElement("unserviced");
		xml.PushAttribute("floor", *i);
		xml.CloseElement();
	}
	for (Cars::iterator c = cars.begin(); c != cars.end(); c++) {
		xml.OpenElement("car");
		(*c)->encodeXML(xml);
		xml.CloseElement();
	}
}

void Elevator::decodeXML(tinyxml2::XMLElement & xml)
{
	clearCars();
	Item::decodeXML(xml);
	size.y = xml.IntAttribute("height");
	for (tinyxml2::XMLElement * e = xml.FirstChildElement("unserviced"); e; e = e->NextSiblingElement("unserviced")) {
		unservicedFloors.insert(e->IntAttribute("floor"));
	}
	for (tinyxml2::XMLElement * e = xml.FirstChildElement("car"); e; e = e->NextSiblingElement("car")) {
		ElevatorCar * car = new ElevatorCar(this);
		car->decodeXML(*e);
		cars.insert(car);
	}
	updateSprite();
}

rectd Elevator::getMouseRegion()
{
	sf::Vector2f p = GetPosition();
	sf::Vector2f s = GetSize();
	return rectd(p.x, p.y - s.y - 36, s.x, s.y + 2*36);
}

void Elevator::repositionMotor(int motor, int y)
{
	assert(motor == -1 || motor == 1);
	int height;
	int newy;
	if (motor == -1) {
		newy = y + 1;
		height = (size.y + position.y - newy);
	} else {
		newy = position.y;
		height = (y - position.y);
	}
	if (height < 1)  height = 1;
	if (height > 30+1) height = 30+1;
	if (motor == -1) {
		newy = (size.y + position.y - height);
	}
	if (newy != position.y || height != size.y) {
		LOG(DEBUG, "relocate elevator: %i, %i", newy, height);
		setPosition(int2(position.x, newy));
		size.y = height;
		//TODO: constrain cars to stay within elevator bounds.
		for (Cars::iterator c = cars.begin(); c != cars.end(); c++) (*c)->reposition();
		updateSprite();
	}
}

void Elevator::clearCars()
{
	for (Cars::iterator c = cars.begin(); c != cars.end(); c++) delete *c;
	cars.clear();
}

void Elevator::addCar(int floor)
{
	ElevatorCar * car = new ElevatorCar(this);
	car->setAltitude(floor);
	cars.insert(car);
}
