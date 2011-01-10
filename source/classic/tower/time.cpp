#include "time.h"

#include "tower.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

TowerTime::TowerTime(Tower * tower) : Engine::Object(), tower(tower)
{
	time = 5;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Time
//----------------------------------------------------------------------------------------------------

double TowerTime::getTime()
{
	return time;
}

void TowerTime::setTime(double t)
{
	if (time != t) {
		time = t;
		tower->background->updateIfNeeded.setNeeded();
	}
}

double TowerTime::getTimeOfDay()
{
	return fmod(getTime(), 24);
}

bool TowerTime::isBetween(double a, double b)
{
	double tod = getTimeOfDay();
	return (tod >= a && tod < b);
}

unsigned int TowerTime::getDate()
{
	return (getTimeOfDay() / 24);
}

unsigned int TowerTime::getDayOfWeek()
{
	return (getDate() % 3);
}

unsigned int TowerTime::getQuarter()
{
	return ((getDate() / 3) % 4);
}

unsigned int TowerTime::getYear()
{
	return (getDate() / 12);
}

bool TowerTime::isWeekday()
{
	return (getDayOfWeek() < 2);
}

bool TowerTime::isWeekend()
{
	return (getDayOfWeek() == 2);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

double TowerTime::getTimeSpeed()
{
	double s = 0.25;
	if (time > 1.5 && time < 6.0)
		s = 1;
	if (time > 12 && time < 13)
		s = 1.0 / 45;
	s *= pow(2, tower->debugSpeed);
	return s;
}

void TowerTime::advance(double dt)
{
	//Advance the game time
	setTime(getTime() + dt * getTimeSpeed());
}
