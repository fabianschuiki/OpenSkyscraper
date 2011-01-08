#include "eventpump.h"

#include "application.h"

using namespace OSS;
using namespace Engine;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

EventPump::EventPump(Application * application) : Core::EventPump(), application(application)
{	
	//Make sure we received keyboard repeat events
	SDL_EnableKeyRepeat(250, 50);
	
	//Supporting Unicode wouldn't hurt either in the 21st centurey
	SDL_EnableUNICODE(SDL_TRUE);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Event Pumping
//----------------------------------------------------------------------------------------------------

Core::Event * EventPump::getNextEvent()
{
	//Fetch the next SDL event
	SDL_Event event;
	if (!SDL_PollEvent(&event))
		return NULL;
	
	//A variable for the interpreted event
	Core::Event * e = NULL;
	
	//Interpret mouse button events
	if (SDL_EVENTMASK(event.type) & (SDL_MOUSEBUTTONDOWNMASK | SDL_MOUSEBUTTONUPMASK)) {
		e = new Core::MouseButtonEvent(int2(event.button.x, event.button.y),
									   event.button.button,
									   (event.button.state == SDL_PRESSED));
	}
	
	//Interpret mouse moved events
	if (SDL_EVENTMASK(event.type) & (SDL_MOUSEMOTIONMASK)) {
		e = new Core::MouseMovedEvent(int2(event.motion.x, event.motion.y),
									  int2(event.motion.xrel, event.motion.yrel));
	}
	
	//TODO: Create some sort of mouse dragged events
	
	//Interpret scroll wheel events
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_WHEELUP)
			e = new Core::ScrollWheelEvent(int2(event.button.x, event.button.y), double2(0, 1));
		if (event.button.button == SDL_BUTTON_WHEELDOWN)
			e = new Core::ScrollWheelEvent(int2(event.button.x, event.button.y), double2(0, -1));
	}
	
	//Interpret key events
	if (SDL_EVENTMASK(event.type) & (SDL_KEYDOWNMASK | SDL_KEYUPMASK)) {
		e = new Core::KeyEvent(event.key.keysym.unicode,
							   (event.key.state == SDL_PRESSED),
							   false);
	}
	
	//Special treatment for quit events
	if (event.type == SDL_QUIT)
		application->terminate();
	
	//Special shortcut for quitting
	if (event.type == SDL_KEYDOWN && event.key.keysym.mod & KMOD_META &&
		event.key.keysym.unicode == 'q')
		application->terminate();
	
	//Autorelease the event if we created one, and return
	if (e) e->autorelease();
	return e;
}
