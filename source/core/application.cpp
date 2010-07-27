#include "application.h"
#include "core.h"

using namespace OSS;


//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

Application::Application()
{
	//Initialize the singletons
	new Platform;
	new OpenGLCanvas;
	new Engine;
}

Application::~Application()
{
	//Delete the singletons
	delete Engine::shared();
	delete OpenGLCanvas::shared();
	delete Platform::shared();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Life Cycle
//----------------------------------------------------------------------------------------------------

void Application::prepare()
{
	int error;
	
	//Initialize the SDL
	error = SDL_Init(SDL_INIT_VIDEO);
	
	//Prepare OpenGL
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, true);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	
	//Prepare the singletons
	OpenGLCanvas::shared()->eventPrepare();
	Engine::shared()->eventPrepare();
}

void Application::run()
{	
	//Runloop
	keepRunning = true;
	while (keepRunning)
		Engine::shared()->runloopCycle();
}

void Application::cleanUp()
{
	//Give the singletons the possibility to perform any post-run cleanup
	Engine::shared()->eventCleanUp();
	OpenGLCanvas::shared()->eventCleanUp();
	
	//Shutdown the SDL
	SDL_Quit();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Shutdown
//----------------------------------------------------------------------------------------------------

void Application::quit()
{
	keepRunning = false;
}
