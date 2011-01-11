#include "video.h"

#include "../application.h"
#include "../event.h"

using namespace OSS;
using namespace Engine;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

Video::Video(Application * application) : Core::Responder(), application(application)
{
	//Initialize the video subsystem
	SDL_InitSubSystem(SDL_INIT_VIDEO);
	
	//Prepare some OpenGL attributes
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, true);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	
	//Setup some default values
	safeMode.resolution = int2(1000, 600);
	safeMode.fullscreen = false;
	desiredMode = safeMode;
	currentMode = safeMode;
	
	//Activate the desired mode
	activateMode();
}

Video::~Video()
{
	//Shut down the video subsystem
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Video Mode
//----------------------------------------------------------------------------------------------------

bool Video::activateMode()
{
	bool success = switchToMode(&desiredMode);
	if (success) {
		//Store the current mode
		SDL_Surface * surface = SDL_GetVideoSurface();
		currentMode.resolution.x = surface->w;
		currentMode.resolution.y = surface->h;
		currentMode.fullscreen = (surface->flags & SDL_FULLSCREEN);
		
		//Issue an event to inform the game about the mode change
		Pointer<VideoEvent> e = new VideoEvent(Event::VideoChanged, this);
		application->sendEvent(e);
	}
	return success;
}

void Video::revertToSafeMode()
{
	assert(switchToMode(&safeMode));
}

void Video::confirmCurrentModeToBeSafe()
{
	safeMode = currentMode;
}

bool Video::switchToMode(VideoMode * mode)
{
	if (!mode) return false;
	
	Uint32 flags = SDL_OPENGL;
	if (mode->fullscreen)
		flags |= SDL_FULLSCREEN;
	else
		flags |= SDL_RESIZABLE;
	
	return (SDL_SetVideoMode(mode->resolution.x, mode->resolution.y, 0, flags) != NULL);
}

void Video::swapBuffers()
{
	SDL_GL_SwapBuffers();
}
