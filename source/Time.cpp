#include <cassert>
#include "Application.h"
#include "Time.h"

using namespace OT;

const static double kDayDuration = 3.2; // minutes / game day
const double Time::kBaseSpeed    = 1.0 / 60 / kDayDuration; // game days / second


double Time::absoluteToHour(double a)
{
	//jcranmer did some research on how time progresses in SimTower (doc/simtower/TDT_format.txt).
	//We scale the time up to the 2600 frames/day he proposed, and derive the hour by applying
	//different hours/frame scaling factors according to the frame.
	double stage = fmod(a*2600, 2600); //"frame" in TDT_format.txt
	double hour;
	if      (stage < 100)  hour = 0  + (stage-   0)/100 *1;
	else if (stage < 300)  hour = 1  + (stage- 100)/200 *6;
	else if (stage < 700)  hour = 7  + (stage- 300)/400 *5;
	else if (stage < 1500) hour = 12 + (stage- 700)/800 *1;
	else if (stage < 2600) hour = 13 + (stage-1500)/1100*11;
	assert(hour >= 0 && hour < 24);
	return hour;
}

double Time::hourToAbsolute(double h)
{
	double a;
	if      (h < 1)  a = (h -  0) /  1 *  100 +    0;
	else if (h < 7)  a = (h -  1) /  6 *  200 +  100;
	else if (h < 12) a = (h -  7) /  5 *  400 +  300;
	else if (h < 13) a = (h - 12) /  1 *  800 +  700;
	else if (h < 24) a = (h - 13) / 11 * 1100 + 1500;
	a /= 2600;
	assert(a >= 0 && a < 1);
	return a;
}

Time::Time()
{
	speed = 1;
	speed_animated = speed;
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
		dta = absolute - prev_absolute;
		hour = absoluteToHour(absolute);
		day = (int)floor(t) % 3;
		quarter = (int)floor(t/3) % 4 + 1;
		year = (int)floor(t/3/4) + 1;
	}
}

void Time::advance(double dt)
{
	if (speed_animated != speed) {
		if (speed < 1) {
			speed_animated = speed;
		} else {
			speed_animated = (speed * dt + speed_animated * 0.1) / (dt + 0.1);
			if (fabs(speed_animated - speed) < 1e-2) speed_animated = speed;
		}
	}
	
	//Advance the absolute time.
	set(absolute + dt * kBaseSpeed * speed_animated);
}

/** Returns true if the time has just passed said absolute time. */
bool Time::check(double a)
{
	return (prev_absolute < a && absolute >= a);
}

/** Returns true if the time has just passed said hour. E.g. if the time advanced from 9.98 to 10.1
 * checkHour(10) would return true. */
bool Time::checkHour(double h)
{
	return (prev_hour < h && hour >= h);
}
