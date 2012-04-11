#include <cassert>
#include <Rocket/Core/Element.h>
#include "Game.h"
#include "ToolboxWindow.h"

using namespace OT;


void ToolboxWindow::close()
{
	buttons.clear();
	if (window) {
		window->RemoveReference();
		window->Close();
	}
	window = NULL;
}

void ToolboxWindow::reload()
{
	close();
	
	window = game->gui.loadDocument("toolbox.rml");
	assert(window);
	window->Show();
	
	//Add the item buttons.
	for (int i = 0; i < game->itemFactory.prototypes.size(); i++) {
		Item::AbstractPrototype * prototype = game->itemFactory.prototypes[i];
		
		char id[128];
		snprintf(id, 128, "item-%s", prototype->id.c_str());
		
		char style[512];
		snprintf(style, 512, "button#%s { background-image-s: %ipx %ipx; }", id, prototype->icon*32, prototype->icon*32+32);
		LOG(DEBUG, "style for %s: %s", prototype->name.c_str(), style);
		
		Rocket::Core::StyleSheet * sheet = Rocket::Core::Factory::InstanceStyleSheetString(style);
		assert(sheet && "unable to instantiate stylesheet");
		window->SetStyleSheet(window->GetStyleSheet()->CombineStyleSheet(sheet));
		sheet->RemoveReference();
		
		Rocket::Core::XMLAttributes attributes;
		attributes.Set("class", "item");
		attributes.Set("id", id);
		
		Rocket::Core::Element * button = Rocket::Core::Factory::InstanceElement(NULL, "button", "button", attributes);
		assert(button && "unable to instantiate button");
		
		window->AppendChild(button);
		button->RemoveReference();
		buttons.push_back(button);
	}
}
