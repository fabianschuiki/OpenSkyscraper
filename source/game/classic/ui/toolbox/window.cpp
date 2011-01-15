#include "window.h"

#include "../game.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

ToolsWindow::ToolsWindow(GameUI * ui) : ui(ui),
layoutSubviewsIfNeeded(this, &ToolsWindow::layoutSubviews, &updateIfNeeded)
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
	layoutSubviewsIfNeeded();
	
	View::update();
}

void ToolsWindow::layoutSubviews()
{
	//Keep a union rect to resize the window in the end
	rectd rect;
	
	//Calculate the offset
	double2 offset(0, (groupButtons.size() / 2) * 32);
	
	//If the count is an odd, start positioning the last button at (0, 0). If it is even, start at
	//(32, 0) and fill backwards (right to left, bottom to top).
	int buttonIndex = 0;
	for (unsigned int i = kNoGroup + 1; i < kMaxGroup; i++) {
		
		//Get the group button
		ToolsGroupButton * button = groupButtons[(ItemGroup)i];
		if (!button)
			continue;
		
		//Calculate the button position
		double2 position((buttonIndex % 2) * 32, (buttonIndex / 2) * -32);
		
		//Position the group button
		button->setFrameOrigin(offset + position);
		rect.unify(button->getFrame());
		
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
