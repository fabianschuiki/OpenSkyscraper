#include "cruisecontrol.h"

using namespace OSS;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

CruiseControl::CruiseControl(Engine * engine) : engine(engine)
{
	//Setup everything to 0
	dt = 0;
	freq = 0;
	idle_dt = 0;
	idle_ratio = 0;
	
	damped_dt = 0;
	damped_freq = 0;
	damped_idle_dt = 0;
	damped_idle_ratio = 0;
	
	previous_frameTime = 0;
	current_frameTime = 0;
	renderingDone_frameTime = 0;
	
	//Setup some basic limiting values
	freq_lowerLimit = 15;
	freq_upperLimit = 60;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Timing
//----------------------------------------------------------------------------------------------------

void CruiseControl::frameStart()
{
	//Enforce the upper frequency limit
	double timeToWaste = 0;
	upperFrequencyLimitEnforced = false;
	do {
		timeToWaste = current_frameTime + (1.0 / freq_upperLimit) - engine->getTimeElapsed();
		if (timeToWaste > 0) {
			engine->sleep(timeToWaste * 0.8);
			upperFrequencyLimitEnforced = true;
		}
	} while (timeToWaste > 0);
	
	//Set the previous frame time and fetch the current one
	previous_frameTime = current_frameTime;
	current_frameTime = engine->getTimeElapsed();
	
	//Calculate the dt of the frame
	dt = current_frameTime - previous_frameTime;
	if (dt < 0 || !isfinite(dt)) dt = 0;
	
	//Enforce the lower frequency limit
	if (dt > 1.0 / freq_lowerLimit) {
		dt = 1.0 / freq_lowerLimit;
		lowerFrequencyLimitEnforced = true;
	} else {
		lowerFrequencyLimitEnforced = false;
	}
	
	//Calculate the frequency
	if (dt > 0)
		freq = 1.0 / dt;
	else
		freq = 0;
	
	//Calculate the idle times
	idle_dt = current_frameTime - renderingDone_frameTime;
	if (idle_dt < 0 || !isfinite(idle_dt)) idle_dt = 0;
	idle_ratio = 1.0 - (idle_dt / dt);
	if (idle_ratio < 0 || !isfinite(idle_ratio)) idle_ratio = 1.0;
	
	//Generate the damped output
	damped_dt = (damped_dt + (dt * dt)) / (dt + 1);
	damped_freq = (damped_freq + (freq * dt)) / (dt + 1);
	damped_idle_dt = (damped_idle_dt + (idle_dt * dt)) / (dt + 1);
	damped_idle_ratio = (damped_idle_ratio + (idle_ratio * dt)) / (dt + 1);
	
	if (!isfinite(damped_dt)) damped_dt = dt;
	if (!isfinite(damped_freq)) damped_freq = freq;
	if (!isfinite(damped_idle_dt)) damped_idle_dt = idle_dt;
	if (!isfinite(damped_idle_ratio)) damped_idle_ratio = idle_ratio;
}

void CruiseControl::renderingDone()
{
	//Take the frame time
	renderingDone_frameTime = engine->getTimeElapsed();
}
