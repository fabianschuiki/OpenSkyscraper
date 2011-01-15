#include "window.h"

#include "../game.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

ToolsWindow::ToolsWindow(GameUI * ui) : Classic::Responder(this), ui(ui),
updateButtonsIfNeeded(this, &ToolsWindow::updateButtons, &updateIfNeeded),
layoutIfNeeded(this, &ToolsWindow::layout, &updateIfNeeded)
{
	//Since the control window is fixed in width, we may set it right now. The height is being calc-
	//ulated dynamically.
	setFrameSize(double2(64, 180));
	
	//Initialize the group buttons
	for (int i = kNoGroup + 1; i < kMaxGroup; i++) {
		ToolsGroupButton * button = new ToolsGroupButton(this, (ItemGroup)i);
		addSubview(button);
		groupButtons[(ItemGroup)i] = button;
	}
}

Tower * ToolsWindow::getTower()
{
	return ui->getTower();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark State
//----------------------------------------------------------------------------------------------------

void ToolsWindow::update()
{
	//Layout the subviews if required
	layoutIfNeeded();
	
	//Update the buttons if required
	updateButtonsIfNeeded();
	
	//Update the subviews etc.
	View::update();
}

void ToolsWindow::updateButtons()
{
	//Update the selection state of the buttons. First we need a pointer to the construction tool
	//since we need that during the process.
	ConstructionTool * constructionTool = ui->tools->constructionTool;
	
	//We'll also need the selected item descriptor.
	ItemDescriptor * desc = constructionTool->getItemDescriptor();
	
	//Iterate through the buttons and decide for each whether it should be selected.
	for (GroupButtonMap::iterator it = groupButtons.begin(); it != groupButtons.end(); it++) {
		
		//If the construction tool is not selected, or the descriptor is invalid, the procedure is
		//trivial.
		if (!constructionTool->isSelected() || !desc) {
			it->second->pressed = false;
			continue;
		}
		
		//If the item group equals the current button's one, mark the button as pressed and select
		//the appropriate item in the button's popup.
		bool groupEquals = (desc->group == it->first);
		it->second->pressed = groupEquals;
		if (groupEquals)
			it->second->setSelectedItem(desc->type);
	}
}

void ToolsWindow::layout()
{
	//Keep a union rect to resize the window in the end
	rectd rect;
	
	//Calculate the offset
	double2 offset(0, (groupButtons.size() / 2) * 32);
	
	//If the count is an odd, start positioning the last button at (0, 0). If it is even, start at
	//(32, 0) and fill backwards (right to left, bottom to top).
	int buttonIndex = 0;
	for (GroupButtonMap::iterator it = groupButtons.begin(); it != groupButtons.end(); it++) {
		
		//Calculate the button position
		double2 position((buttonIndex % 2) * 32, (buttonIndex / 2) * -32);
		
		//Position the group button
		it->second->setFrameOrigin(offset + position);
		rect.unify(it->second->getFrame());
		
		//Increase the button index
		buttonIndex++;
	}
	
	//Position bulldozer etc. right above the last button. Maybe use some union rect to do that.
	
	//Position play/pause controls right above the previous. If its location changed, update the
	//window's height to cover the entire contents and nothing more.
	
	//Set the window's size
	setFrameSize(rect.size);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

void ToolsWindow::draw(rectd dirtyRect)
{
	//Fill the area of the window with some debugging color
	TexturedQuad quad;
	quad.rect.size = getFrameSize();
	quad.color = (color4d){0.8, 0.8, 0.8, 1};
	quad.draw();
	
	//Call the original implementation of this method which will also draw the subviews
	View::draw(dirtyRect);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Events
//----------------------------------------------------------------------------------------------------

void ToolsWindow::eventToolChanged(Event * event)
{
	//Update the buttons
	updateButtonsIfNeeded.setNeeded();
}
