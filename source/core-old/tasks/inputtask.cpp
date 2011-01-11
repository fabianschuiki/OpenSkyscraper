#include "inputtask.h"
#include "../core.h"

using namespace OSS;


void InputTask::update()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		CoreEvent e;
		EventSDL * sdl = &e.base.sdl;
		sdl->type = kEventSDL;
		sdl->event = &event;
		if (SDL_EVENTMASK(event.type) & (SDL_MOUSEBUTTONDOWNMASK | SDL_MOUSEBUTTONUPMASK))
			event.button.y = OpenGLCanvas::shared()->currentMode.resolution.y - event.button.y;
		Application::shared()->handleEvent(&e);
	}
}
