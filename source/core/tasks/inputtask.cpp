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
		Engine::shared()->handleEvent(&e);
	}
}
