#include <cassert>
#ifdef BUILD_DEBUG
#include <Rocket/Debugger/Debugger.h>
#endif

#include "Application.h"
#include "GUI.h"
#include "GUIManager.h"
#include "GL/gl.h"

using namespace OT;


GUI::GUI(std::string name, GUIManager * manager)
{
	assert(manager && "GUI requires a GUIManager");
	
	this->manager = manager;
	context = Rocket::Core::CreateContext(name.c_str(),
					Rocket::Core::Vector2i(
						manager->window->getView().getSize().x,
						manager->window->getView().getSize().y));
	assert(context && "unable to initialize context");
}

GUI::~GUI()
{
	context->RemoveReference();
	context = NULL;
}

bool GUI::handleEvent(sf::Event & event)
{
	switch (event.type) {
		case sf::Event::Resized:
			this->context->SetDimensions(Rocket::Core::Vector2i(
											manager->window->getView().getSize().x,
											manager->window->getView().getSize().y));
			return true;
		case sf::Event::MouseMoved:
			context->ProcessMouseMove(event.mouseMove.x, event.mouseMove.y, manager->getKeyModifiers());
			return true;
		case sf::Event::MouseButtonPressed:
			context->ProcessMouseButtonDown(event.mouseButton.button, manager->getKeyModifiers());
			return true;
		case sf::Event::MouseButtonReleased:
			context->ProcessMouseButtonUp(event.mouseButton.button, manager->getKeyModifiers());
			return true;
		case sf::Event::MouseWheelMoved:
			return context->ProcessMouseWheel(event.mouseWheel.delta, manager->getKeyModifiers());
		case sf::Event::TextEntered:
			return context->ProcessTextInput(event.text.unicode);
		case sf::Event::KeyPressed:
			return context->ProcessKeyDown(manager->translateKey(event.key.code), manager->getKeyModifiers());
		case sf::Event::KeyReleased:
			return context->ProcessKeyUp(manager->translateKey(event.key.code), manager->getKeyModifiers());
	}
	return false;
}

void GUI::draw()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, manager->window->getView().getSize().x, manager->window->getView().getSize().y, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	context->Update();
	context->Render();
	
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

Rocket::Core::ElementDocument * GUI::loadDocument(Path path)
{
	DataManager::Paths paths = App->data.paths(Path("gui").down(path));
	for (DataManager::Paths::iterator p = paths.begin(); p != paths.end(); p++) {
		Rocket::Core::ElementDocument * document = context->LoadDocument((*p).c_str());
		if (document) {
			LOG(DEBUG, "loaded GUI document '%s'", path.c_str());
			return document;
		}
	}
	LOG(ERROR, "unable to load GUI document '%s'", path.c_str());
	return NULL;
}
