#include "State.h"

using namespace OT;


State::State()
{
	gui = NULL;
	active = false;
}

void State::activate()
{
	if (!active && gui) gui->Show();
	active = true;
}

void State::deactivate()
{
	if (active && gui) gui->Hide();
	active = false;
}

void State::setGUI(Rocket::Core::ElementDocument * gui)
{
	if (this->gui != gui) {
		if (this->gui && active) this->gui->Hide();
		this->gui = gui;
		if (this->gui && active) this->gui->Show();
	}
}
