#include <cassert>
#ifdef BUILD_DEBUG
#include <Rocket/Debugger/Debugger.h>
#endif

#include "application.h"
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
	
	this->context = Rocket::Core::CreateContext("default", Rocket::Core::Vector2i(window->GetWidth(), window->GetHeight()));
	
#ifdef BUILD_DEBUG
	Rocket::Debugger::Initialise(context);
#endif
	
	return true;
}

bool GUIManager::handleEvent(sf::Event & event)
{
	assert(this->context != NULL && "Rocket context required for event handling");
	
	switch (event.Type) {
		case sf::Event::Resized:
			this->context->SetDimensions(Rocket::Core::Vector2i(window->GetWidth(), window->GetHeight()));
			return true;
		case sf::Event::MouseMoved:
			context->ProcessMouseMove(event.MouseMove.X, event.MouseMove.Y, system.GetKeyModifiers(window));
			return true;
		case sf::Event::MouseButtonPressed:
			context->ProcessMouseButtonDown(event.MouseButton.Button, system.GetKeyModifiers(window));
			return true;
		case sf::Event::MouseButtonReleased:
			context->ProcessMouseButtonUp(event.MouseButton.Button, system.GetKeyModifiers(window));
			return true;
		case sf::Event::MouseWheelMoved:
			return context->ProcessMouseWheel(event.MouseWheel.Delta, system.GetKeyModifiers(window));
		case sf::Event::TextEntered:
			return context->ProcessTextInput(event.Text.Unicode);
		case sf::Event::KeyPressed:
#ifdef BUILD_DEBUG
			if (event.Key.Code == sf::Key::F8) {
				bool visible = !Rocket::Debugger::IsVisible();
				LOG(DEBUG, "Rocket::Debugger %s", (visible ? "on" : "off"));
				Rocket::Debugger::SetVisible(visible);
			}
#endif
			return context->ProcessKeyDown(system.TranslateKey(event.Key.Code), system.GetKeyModifiers(window));
		case sf::Event::KeyReleased:
			return context->ProcessKeyUp(system.TranslateKey(event.Key.Code), system.GetKeyModifiers(window));
	}
	return false;
}

void GUIManager::draw()
{
	context->Update();
	context->Render();
}
