#include "engine.h"

#include "application.h"
#include "resources/sound.h"
#include "resources/texture.h"
#include "scene.h"

using namespace OSS;
using namespace Engine;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

EngineCore::EngineCore(Application * application)
{
	//Initialize the stores
	stores.push_back(new TextureStore);
	stores.push_back(new SoundStore);
	
	//Initialize the cruise control which times the animation and slows the loop if required
	timing = new CruiseControl(this);
	
	//Attach the engine to the application's run loop
	attachToApplication(application);
}

EngineCore::~EngineCore()
{
	//Detach us from the application
	detachFromApplication();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Run Loop Attachment
//----------------------------------------------------------------------------------------------------

void EngineCore::attachToApplication(Application * app)
{
	//Detach from the current application
	detachFromApplication();
	if (!app) return;
	
	//Set the new application
	application = app;
	
	//Create the new invocation and add it to the application
	invocation = new Core::Invocation<EngineCore>(this, &EngineCore::EngineCore::update);
	invocation->release();
	application->addInvocation(invocation);
}

void EngineCore::detachFromApplication()
{
	//Remove the invocation from the application
	if (application)
		application->removeInvocation(invocation);
	
	//Delete the invocation and release the application
	invocation = NULL;
	application = NULL;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark State
//----------------------------------------------------------------------------------------------------

void EngineCore::update()
{
	//Perform cruise control
	timing->frameStart();
	
	//Setup the current instances
	application->pushCurrent();
	application->audio->pushCurrent();
	application->video->pushCurrent();
	
	//Load stuff
	performLoadingAndFinalizing();
	
	//Do the magic
	simulateScene();
	updateScene();
	drawScene();
	
	//Release the current instances
	application->popCurrent();
	application->audio->popCurrent();
	application->video->popCurrent();
		
	//We're done rendering
	timing->renderingDone();
	
	//Show some basic information in the window title
	char title[512];
	snprintf(title, 512, "OpenSkyscraper | %3.0f Hz, %3.0f ms, %3.0f%%",
			 timing->damped_freq,
			 timing->damped_dt * 1000,
			 timing->damped_idle_ratio * 100);
	SDL_WM_SetCaption(title, NULL);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Scene
//----------------------------------------------------------------------------------------------------

Scene * EngineCore::getScene()
{
	return scene;
}

void EngineCore::setScene(Scene * scene)
{
	willSwitchToScene(scene);
	
	//Notify the scene that it gets moved off screen
	if (this->scene) {		
		this->scene->willMoveOffScreen();
		
		Event * e = new Event(Event::MovedOffScreen);
		this->scene->sendEvent(e);
		e->release();
		
		this->scene->didMoveOffScreen();
	}
	
	//Set the new scene
	this->scene = scene;
	
	//Notify the scene that it gets moved on screen
	if (this->scene) {		
		this->scene->willMoveOnScreen();
		
		Event * e = new Event(Event::MovedOnScreen);
		this->scene->sendEvent(e);
		e->release();
		
		this->scene->didMoveOnScreen();
	}
	
	didSwitchToScene(scene);
}

void EngineCore::simulateScene()
{
	if (scene) scene->advance(timing->dt);
}

void EngineCore::updateScene()
{
	if (scene) scene->update();
}

void EngineCore::drawScene()
{
	if (scene) {
		scene->draw();
	} else {
		glClearColor(0, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Stores
//----------------------------------------------------------------------------------------------------

void EngineCore::performLoadingAndFinalizing()
{
	double start = application->getTimeElapsed();
	double now = start;
	for (StoreList::iterator store = stores.begin(); store != stores.end() && (now - start < 0.01);
		 store++) {
		bool workDone = false;
		while (!workDone && (now - start < 0.01)) {
			now = application->getTimeElapsed();
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
#pragma mark Event Sending
//----------------------------------------------------------------------------------------------------

bool EngineCore::sendEventToNextResponders(Base::Event * event)
{
	if (scene && scene->sendEvent(event)) return true;
	return Core::Responder::sendEventToNextResponders(event);
}
