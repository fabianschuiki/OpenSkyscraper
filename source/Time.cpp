#include <cassert>
#include "Application.h"
#include "Time.h"

using namespace OT;

const static double kDayDuration = 3.2; // minutes / game day
const static double kBaseSpeed   = 1.0 / 60 / kDayDuration; // game days / second


Time::Time()
{
	speed = 1;
	speed_animated = speed;
	speedMode = 1;
}

void Time::set(double t)
{
	if (absolute != t) {
		prev_absolute = absolute;
		prev_hour = hour;
		prev_day = day;
		prev_quarter = quarter;
		prev_year = year;
		
		absolute = t;
		hour = fmod(t, 24);
		day = (int)floor(t/24) % 3;
		quarter = (int)floor(t/24/3) % 4 + 1;
		year = (int)floor(t/24/3/4) + 1;
	}
}

void Time::advance(double dt)
{
	if (speed_animated != speed) {
		if (speed < 1) {
			speed_animated = speed;
		} else {
			speed_animated = (speed * dt + speed_animated) / (dt + 1);
			if (fabs(speed_animated - speed) < 1e-2) speed_animated = speed;
		}
	}
	
	//Advance the absolute time.
	set(absolute + dt * kBaseSpeed * speed_animated);
	
	//According to jcranmer's investigations on the time subject, we know that the original game
	//used to measure the day in frames which accorded to seconds on a different basis depending on
	//the time of day.
	
	//Define the basic speed of the game. This is the speed at which time will pass at lunch.
	double speed = 1.0/60; //game hours per second
	
	//Apply a multiplication factor derived from the original game (which measured this in
	//seconds/frame).
	if      (hour >= 7  && hour < 12) speed *= 10; //7:00  to 12:00, 45s/frame
	else if (hour >= 12 && hour < 13) speed *= 1;  //12:00 to 13:00, 4.5s/frame
	else if (hour >= 13 || hour < 1)  speed *= 8;  //13:00 to 1:00,  36s/frame
	else 	                          speed *= 28; //1:00 to 7:00, 126s/frame
	
	//Advance the time.
	//set(absolute + dt*speed);
}

void Time::setSpeedMode(int sm)
{
	assert(sm >= 0 && sm <= 3);
	if (speedMode != sm) {
		speedMode = sm;
		switch (speedMode) {
			case 0: speed = 0; break;
			case 1: speed = 1; break;
			case 2: speed = 2; break;
			case 3: speed = 4; break;
		}
	}
}

/** Returns true if the time has just passed said hour. E.g. if the time advanced from 9.98 to 10.1
 * checkHour(10) would return true. */
bool Time::checkHour(double t)
{
	return (prev_hour < t && hour >= t);
}
