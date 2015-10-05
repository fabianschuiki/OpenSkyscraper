#include <cassert>
#include <Rocket/Controls.h>
#include <Rocket/Debugger.h>

#include "Application.h"
#include "GUIManager.h"

using namespace OT;

GUIManager::GUIManager()
{
	window = NULL;
}

GUIManager::~GUIManager()
{
	//TODO: do cleanup here.
}

bool GUIManager::init(sf::RenderWindow * window)
{
	assert(window != NULL && "window must not be NULL");

	this->window = window;
	renderer.SetWindow(window);

	Rocket::Core::SetRenderInterface(&renderer);
	Rocket::Core::SetSystemInterface(&system);

	if (!Rocket::Core::Initialise()) {
		LOG(ERROR, "unable to initialize Rocket::Core");
		return false;
	}
	Rocket::Controls::Initialise();

	return true;
}

Rocket::Core::Input::KeyIdentifier GUIManager::translateKey(sf::Keyboard::Key key)
{
	return system.TranslateKey(key);
}

int GUIManager::getKeyModifiers()
{
	return system.GetKeyModifiers(window);
}
