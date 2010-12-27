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
	
	//Make sure we received keyboard repeat events
	SDL_EnableKeyRepeat(250, 50);
	
	//Prepare OpenGL
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, true);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	
	//Initialize DevIL
	ilInit();
	
	//Tell the DevIL how all loaded images should be oriented
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	
	//DEBUG: Setup some OpenAL context so we can at least play sounds
	ALCdevice * device = alcOpenDevice(NULL);
	OSSObjectLog << "initializing audio device " << alcGetString(device, ALC_DEVICE_SPECIFIER) << std::endl;
	ALCcontext * context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);
	
	//Prepare the singletons
	OpenGLCanvas::shared()->eventPrepare();
	Engine::shared()->eventPrepare();
	
	//Give subclasses a chance to prepare
	onPrepare();
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
	//Give subclasses a chance to clean up before we get to work
	onCleanUp();
	
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
