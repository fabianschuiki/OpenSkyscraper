#include "State.h"
#include "Application.h"

using namespace OT;


State::State() : gui(&App->gui)
{
	active = false;
}

void State::activate()
{
	active = true;
}

void State::deactivate()
{
	active = false;
}
