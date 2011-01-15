#include "time.h"

#include "tower.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

TowerTime::TowerTime(Tower * tower) : tower(tower)
{
	time = 5;
	
	previousTime = time;
	bufferedPreviousTime = previousTime;
	
	paused = false;
	debugSpeed = 0;
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
		unsigned int previousDay = getDayOfWeek();
		unsigned int previousQuarter = getQuarter();
		unsigned int previousYear = getYear();
		
		//Store the time
		time = t;
		
		//Distribute the time changed event
		tower->sendEvent(new Event(Event::kTimeChanged));
		
		//Distribute the day changed event
		if (getDayOfWeek() != previousDay)
			tower->sendEvent(new Event(Event::kDayChanged));
		
		//Distribute the quarter changed event
		if (getQuarter() != previousQuarter)
			tower->sendEvent(new Event(Event::kQuarterChanged));
		
		//Distribute the year changed event
		if (getYear() != previousYear)
			tower->sendEvent(new Event(Event::kYearChanged));
	}
}

double TowerTime::getTimeOfDay()
{
	return fmod(getTime(), 24);
}

bool TowerTime::isAfter(double a)
{
	return (getTimeOfDay() >= a);
}

bool TowerTime::isBetween(double a, double b)
{
	double tod = getTimeOfDay();
	return (tod >= a && tod < b);
}

bool TowerTime::isBefore(double b)
{
	return (getTimeOfDay() < b);
}

unsigned int TowerTime::getDate()
{
	return (getTime() / 24);
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
#pragma mark Triggering
//----------------------------------------------------------------------------------------------------

bool TowerTime::check(double alarmTime)
{
	return (getTime() >= alarmTime && previousTime < alarmTime);
}

bool TowerTime::checkDaily(double alarmTime)
{
	return (getTimeOfDay() >= alarmTime && fmod(previousTime, 24) < alarmTime);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

bool TowerTime::isPaused()
{
	return paused;
}

void TowerTime::setPaused(bool p)
{
	if (paused != p) {
		paused = p;
		//TODO: Maybe send some event here?
	}
}

double TowerTime::getTimeSpeed()
{
	double s = 0.25;
	double tod = getTimeOfDay();
	if (tod > 1.5 && tod < 6.0)
		s = 1;
	if (tod > 12 && tod < 13)
		s = 1.0 / 45;
	s *= pow(2, debugSpeed);
	return s;
}

void TowerTime::advance(double dt)
{	
	//Advance the game time
	setTime(getTime() + dt * getTimeSpeed());
	
	//Adjust the previous times
	previousTime = bufferedPreviousTime;
	bufferedPreviousTime = getTime();
}
