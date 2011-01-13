#include "engine.h"

#include "resources/sound.h"
#include "resources/texture.h"

using namespace OSS;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

Engine::Engine(Application * application) : application(application)
{
	//Initialize the stores
	stores.push_back(new TextureStore);
	stores.push_back(new SoundStore);
	
	//Initialize the cruise control which times the animation and slows the loop if required
	timing = new CruiseControl(this);
	
	//Initialize the other subsystems
	audio = new Audio(this);
	video = new Video(this);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Subsystems
//----------------------------------------------------------------------------------------------------

void Engine::heartbeat()
{
	//Perform cruise control
	timing->frameStart();
	
	//Load stuff
	performLoadingAndFinalizing();
	
	//Do the magic
	simulateScene();
	updateScene();
	drawScene();
	
	//Update the audio subsystem
	audio->update();
		
	//We're done rendering
	timing->renderingDone();
	
	//Show some basic information in the window title
	char title[512];
	snprintf(title, 512, "OpenSkyscraper | %3.0f Hz, %3.0f ms, %3.0f%%",
			 timing->damped_freq,
			 timing->damped_dt * 1000,
			 timing->damped_idle_ratio * 100);
	SDL_WM_SetCaption(title, NULL);
	
	//Swap the OpenGL buffers
	video->swapBuffers();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Timing
//----------------------------------------------------------------------------------------------------

double Engine::getTimeElapsed()
{
	return ((double)SDL_GetTicks()) / 1000;
}

void Engine::sleep(double seconds)
{
	SDL_Delay(seconds * 1000);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Scene
//----------------------------------------------------------------------------------------------------

Scene * Engine::getScene()
{
	return scene;
}

void Engine::setScene(Scene * scene)
{
	willSwitchToScene(scene);
	
	//Notify the scene that it gets moved off screen
	if (this->scene) {		
		this->scene->willMoveOffScreen();
		
		Event * e = new Event(Event::kMovedOffScreen);
		this->scene->sendEvent(e);
		e->release();
		
		this->scene->didMoveOffScreen();
	}
	
	//Set the new scene
	this->scene = scene;
	
	//Notify the scene that it gets moved on screen
	if (this->scene) {		
		this->scene->willMoveOnScreen();
		
		Event * e = new Event(Event::kMovedOnScreen);
		this->scene->sendEvent(e);
		e->release();
		
		this->scene->didMoveOnScreen();
	}
	
	didSwitchToScene(scene);
}

void Engine::simulateScene()
{
	if (scene)
		scene->advance(timing->dt);
}

void Engine::updateScene()
{
	if (scene)
		scene->update();
}

void Engine::drawScene()
{
	if (scene) {
		scene->draw(rectd(double2(), video->currentMode.resolution));
	} else {
		glClearColor(0, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Stores
//----------------------------------------------------------------------------------------------------

void Engine::performLoadingAndFinalizing()
{
	double start = getTimeElapsed();
	double now = start;
	for (StoreList::iterator store = stores.begin(); store != stores.end() && (now - start < 0.01);
		 store++) {
		bool workDone = false;
		while (!workDone && (now - start < 0.01)) {
			now = getTimeElapsed();
			if ((*store)->unfinalizeNext()) continue;
			if ((*store)->unloadNext()) continue;
			if ((*store)->loadNext()) continue;
			if ((*store)->finalizeNext()) continue;
			workDone = true;
		}
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Events
//----------------------------------------------------------------------------------------------------

bool Engine::sendEventToNextResponders(Event * event)
{
	if (scene && scene->sendEvent(event)) return true;
	return BasicResponder::sendEventToNextResponders(event);
}
