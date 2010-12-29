#include "openglcanvas.h"


using namespace OSS;


//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

OpenGLCanvas::OpenGLCanvas()
{
	//Choose default resolutions
	safeMode.resolution = int2(1024, 768);
	safeMode.fullscreen = false;
	desiredMode = safeMode;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Video Mode
//----------------------------------------------------------------------------------------------------

bool OpenGLCanvas::activateMode()
{
	bool success = switchToMode(&desiredMode);
	if (success)
		currentMode = desiredMode;
	return success;
}

void OpenGLCanvas::revertToSafeMode()
{
	assert(switchToMode(&safeMode));
}

void OpenGLCanvas::confirmCurrentModeToBeSafe()
{
	safeMode = currentMode;
}

bool OpenGLCanvas::switchToMode(VideoMode * mode)
{
	if (!mode) return false;
	
	Uint32 flags = SDL_OPENGL;
	if (mode->fullscreen)
		flags |= SDL_FULLSCREEN;
	else
		flags |= SDL_RESIZABLE;
	
	return (SDL_SetVideoMode(mode->resolution.x, mode->resolution.y, 0, flags) != NULL);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Events
//----------------------------------------------------------------------------------------------------

void OpenGLCanvas::eventPrepare()
{
	//TODO: read the desired and safe modes from the user defaults...
	
	//Activate the desired mode
	assert(activateMode());
}

void OpenGLCanvas::eventCleanUp()
{
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Uncategorized
//----------------------------------------------------------------------------------------------------

void OpenGLCanvas::swapBuffers()
{
	SDL_GL_SwapBuffers();
}
