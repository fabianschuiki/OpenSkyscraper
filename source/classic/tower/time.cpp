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
	previousTime = time;
	bufferedPreviousTime = previousTime;
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
		
		//Adjust the previous times
		previousTime = bufferedPreviousTime;
		bufferedPreviousTime = t;
		
		//Store the time
		time = t;
		OSSObjectLog << t << std::endl;
		
		//Distribute the time changed event
		Event * e = new Event(Event::TimeChanged);
		tower->sendEvent(e);
		e->release();
		
		//Distribute the day changed event
		if (getDayOfWeek() != previousDay) {
			e = new Event(Event::DayChanged);
			tower->sendEvent(e);
			e->release();
		}
		
		//Distribute the quarter changed event
		if (getQuarter() != previousQuarter) {
			e = new Event(Event::QuarterChanged);
			tower->sendEvent(e);
			e->release();
		}
		
		//Distribute the year changed event
		if (getYear() != previousYear) {
			e = new Event(Event::YearChanged);
			tower->sendEvent(e);
			e->release();
		}
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
#pragma mark Triggering
//----------------------------------------------------------------------------------------------------

bool TowerTime::check(double alarmTime)
{
	return (getTime() >= alarmTime && previousTime < alarmTime);
}

bool TowerTime::checkDaily(double alarmTime)
{
	return (getTimeOfDay() >= alarmTime && previousTime < alarmTime);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

double TowerTime::getTimeSpeed()
{
	double s = 0.25;
	double tod = getTimeOfDay();
	if (tod > 1.5 && tod < 6.0)
		s = 1;
	if (tod > 12 && tod < 13)
		s = 1.0 / 45;
	s *= pow(2, tower->debugSpeed);
	return s;
}

void TowerTime::advance(double dt)
{
	//Advance the game time
	setTime(getTime() + dt * getTimeSpeed());
}
