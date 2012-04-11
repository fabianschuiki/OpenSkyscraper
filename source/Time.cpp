#include "Application.h"
#include "Time.h"

using namespace OT;


void Time::set(double t)
{
	if (absolute != t) {
		absolute = t;
		hour = fmod(t, 24);
		day = (int)floor(t/24) % 3;
		quarter = (int)floor(t/24/3) % 4 + 1;
		year = (int)floor(t/24/3/4) + 1;
	}
}

void Time::advance(double dt)
{
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
	set(absolute + dt*speed);
}
