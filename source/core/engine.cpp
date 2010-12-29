#include "application.h"
#include "engine.h"
#include "platform.h"
#include "scene.h"

#include "../resources/resources.h"


using namespace OSS;


//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

Engine::Engine() {
	//Clear the scene
	scene = NULL;
	
	//Initialize timing
	dt = 0;
	freq = 0;
	idle_dt = 0;
	idle_ratio = 0;
	previous_frameTime = 0;
	current_frameTime = 0;
	renderingDone_frameTime = 0;
	freq_lowerLimit = 15;
	freq_upperLimit = 100;
	
	//Initialize the stores
	stores.push_back(new TextureStore);
}

Engine::~Engine() {
	switchToScene(NULL);
	
	//Get rid of the stores
	stores.clear();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Scene
//----------------------------------------------------------------------------------------------------

void Engine::switchToScene(Scene * newScene)
{
	if (scene) {
		scene->onMoveOffScreen();
		
		CoreEvent e;
		e.type = kCoreEventSceneMovedOffScreen;
		e.scene.scene = scene;
		handleEvent(&e);
	}
	
	scene = newScene;
	
	if (scene) {
		scene->onMoveOnScreen();
		
		CoreEvent e;
		e.type = kCoreEventSceneMovedOnScreen;
		e.scene.scene = scene;
		handleEvent(&e);
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Runloop
//----------------------------------------------------------------------------------------------------

void Engine::runloopCycle()
{
	//Update the timing
	timingFrameStart();
	
	//Update the tasks
	inputTask.update();
	simulationTask.update();
	renderTask.update();
	loaderTask.update();
	
	//Take the time now that we're done rendering
	timingRenderingDone();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Events
//----------------------------------------------------------------------------------------------------

bool Engine::handleEvent(CoreEvent * event)
{
	if (event->type != kCoreEventPrepare && event->type != kCoreEventCleanUp) //we're not responsible for this
		if (scene && scene->handleEvent(event)) return true;
	
	if (inputTask.handleEvent(event)) return true;
	if (simulationTask.handleEvent(event)) return true;
	if (renderTask.handleEvent(event)) return true;
	if (loaderTask.handleEvent(event)) return true;
	return CoreObject::handleEvent(event);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Event Handlers
//----------------------------------------------------------------------------------------------------

bool Engine::eventSDL(SDL_Event * event)
{
	switch (event->type) {
		case SDL_QUIT:
			Application::shared()->quit();
			return true;
			break;
	}
	return false;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Timing
//----------------------------------------------------------------------------------------------------

void Engine::timingFrameStart()
{
	//Enforce the upper frequency limit
	double timeToWaste = 0;
	upperFrequencyLimitEnforced = false;
	do {
		timeToWaste = current_frameTime + (1.0 / freq_upperLimit) - Platform::shared()->getTimeElapsed();
		if (timeToWaste > 0) {
			Platform::shared()->sleep(timeToWaste * 0.8);
			upperFrequencyLimitEnforced = true;
		}
	} while (timeToWaste > 0);
	
	//Set the previous frame time and fetch the current one
	previous_frameTime = current_frameTime;
	current_frameTime = Platform::shared()->getTimeElapsed();
	
	//Calculate the dt of the frame
	dt = current_frameTime - previous_frameTime;
	if (dt < 0 || isnan(dt)) dt = 0;
	
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
	idle_ratio = 1.0 - (idle_dt / dt);
}

void Engine::timingRenderingDone()
{
	//Take the frame time
	renderingDone_frameTime = Platform::shared()->getTimeElapsed();
}
