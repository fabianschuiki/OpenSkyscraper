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
	//the time of day. So for a basic scaling of speeds, we assume that the original game used to
	//run at 15 Hz in order to convert the seconds/frame into gamehours/second.
	
	//Define a scaling value which we will multiply the seconds/frame by in order to get the unit we
	//want for our game engine.
	const double c = 15.0 / 3600;
	double f;
	
	//7:00 to 12:00, 45s/frame
	if (hour >= 7 && hour < 12) {
		f = 45 * c;
	}
	
	//12:00 to 13:00, 4.5s/frame
	else if (hour >= 12 && hour < 13) {
		f = 4.5 * c;
	}
		
	//13:00 to 1:00, 36s/frame
	else if (hour >= 13 || hour < 1) {
		f = 36 * c;
	}
	
	//1:00 to 7:00, 126s/frame
	else {
		f = 126 * c;
	}
	
	//Advance the time.
	set(absolute + dt*f*10);
}
