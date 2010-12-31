#include "application.h"
#include "engine.h"
#include "openglcanvas.h"
#include "platform.h"

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
	
	//Supporting Unicode wouldn't hurt either in the 21st centurey
	SDL_EnableUNICODE(SDL_TRUE);
	
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
	alListenerfv(AL_POSITION, (ALfloat[]){0, 0, 0});
	alListenerfv(AL_VELOCITY, (ALfloat[]){0, 0, 0});
	alListenerfv(AL_ORIENTATION, (ALfloat[]){0, 0, 1,  0, 1, 0});
	
	//Prepare the singletons
	//OpenGLCanvas::shared()->sendPrepare();
	//Engine::shared()->sendPrepare();
	
	//Give subclasses a chance to prepare
	sendPrepare();
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
	sendCleanUp();
	
	//Give the singletons the possibility to perform any post-run cleanup
	//Engine::shared()->sendCleanUp();
	//OpenGLCanvas::shared()->sendCleanUp();
	
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





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Events
//----------------------------------------------------------------------------------------------------

bool Application::handleEvent(CoreEvent * event)
{
	if (OpenGLCanvas::shared()->handleEvent(event)) return true;
	if (Engine::shared() && Engine::shared()->handleEvent(event)) return true;
	return CoreObject::handleEvent(event);
}
