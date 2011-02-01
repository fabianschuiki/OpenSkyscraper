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

double TowerTime::getLogicalTimeOfDay()
{
	return fmod(getTime() - 1.5, 24) + 1.5;
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

double TowerTime::getStartOfDay()
{
	return floor(getTime() / 24) * 24;
}

double TowerTime::getLogicalStartOfDay()
{
	return floor((getTime() - 1.5) / 24) * 24;
}

double TowerTime::getTodayRandom(double a, double b)
{
	return randd(maxd(getTimeOfDay(), a), b) + getStartOfDay();
}

double TowerTime::getLogicalTodayRandom(double a, double b)
{
	return randd(maxd(getLogicalTimeOfDay(), a), b) + getLogicalStartOfDay();
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
	//First of all we need the time of day to calculate the speeds.
	double tod = getTimeOfDay();
	
	//According to jcranmer's investigations on the time subject, we know that the original game
	//used to measure the day in frames which accorded to seconds on a different basis depending on
	//the time of day. So for a basic scaling of speeds, we assume that the original game used to
	//run at 15 Hz in order to convert the seconds/frame into gamehours/second.
	double f = 0.25;
	
	//Define a scaling value which we will multiply the seconds/frame by in order to get the unit we
	//want for our game engine.
	const double c = 15.0 / 3600;
	
	//7:00 to 12:00, 45s/frame
	if (tod >= 7 && tod < 12)
		f = 45 * c;
	
	//12:00 to 13:00, 4.5s/frame
	else if (tod >= 12 && tod < 13)
		f = 4.5 * c;
	
	//13:00 to 1:00, 36s/frame
	else if (tod >= 13 || tod < 1)
		f = 36 * c;
	
	//1:00 to 7:00, 126s/frame
	else
		f = 126 * c;
	
	//For debuggin purposes, scale the speed by some amount.
	f *= pow(2, debugSpeed);
	return f;
}

void TowerTime::advance(double dt)
{	
	//Advance the game time
	//TODO: Add some value by which the the entire game speed is scaled in order to allow for the
	//game to be sped up as a feature. Like the old "fast mode".
	setTime(getTime() + dt * getTimeSpeed());
	
	//Adjust the previous times
	previousTime = bufferedPreviousTime;
	bufferedPreviousTime = getTime();
}
