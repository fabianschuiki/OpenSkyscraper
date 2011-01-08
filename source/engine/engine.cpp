#include "engine.h"

#include "application.h"

using namespace OSS;
using namespace Engine;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

EngineCore::EngineCore(Application * application)
{
	//Initialize the cruise control which times the animation and slows the loop if required
	timing = new CruiseControl(this);
	
	attachToApplication(application);
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
		
	//We're done rendering
	timing->renderingDone();
	
	//Show some basic information in the window title
	char title[512];
	sprintf(title, "OpenSkyscraper | %3.0f Hz, %3.0f ms, %3.0f%%",
			timing->damped_freq,
			timing->damped_dt * 1000,
			timing->damped_idle_ratio * 100);
	SDL_WM_SetCaption(title, NULL);
}
