#include "application.h"

using namespace OSS;
using namespace Engine;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

Application::Application()
{	
	//Initialize the renderers
	video = new Video(this);
	audio = new Audio(this);
	
	//Initialize the event pump (requires the video mode to already have the SDL video subsystem
	//initialized)
	eventPump = new EventPump(this);
	
	//Initialize DevIL
	ilInit();
	ilEnable(IL_FILE_OVERWRITE);
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	
	//Hook the update function for the audio task into the run loop
	Core::Invocation<Audio> * audioUpdate = new Core::Invocation<Audio>(audio, &Engine::Audio::update);
	addInvocation(audioUpdate);
	//audioUpdate->release();
	
	//Initialize the engine
	engine = new EngineCore(this);
	
	//Hook the OpenGL buffer swap into the run loop
	Core::Invocation<Video> * inv = new Core::Invocation<Video>(video, &Engine::Video::swapBuffers);
	addInvocation(inv);
	inv->release();
}

Application::~Application()
{
	//Release the engine
	engine = NULL;
	
	//Release DevIL
	ilShutDown();
	
	//Release the renderers
	video = NULL;
	audio = NULL;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Event Sending
//----------------------------------------------------------------------------------------------------

bool Application::sendEventToNextResponders(Base::Event * event)
{
	if (video && video->sendEvent(event)) return true;
	if (audio && audio->sendEvent(event)) return true;
	
	if (engine && engine->sendEvent(event)) return true;
	
	return Core::Application::sendEventToNextResponders(event);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Timing
//----------------------------------------------------------------------------------------------------

double Application::getTimeElapsed()
{
	return ((double)SDL_GetTicks()) / 1000;
}

void Application::sleep(double seconds)
{
	SDL_Delay(seconds * 1000);
}
