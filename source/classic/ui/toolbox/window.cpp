#include "window.h"

#include "../game.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

ToolsWindow::ToolsWindow(GameUI * ui) : GUI::View(), ui(ui)
{
	//Since the control window is fixed in width, we may set it right now. The height is being calc-
	//ulated dynamically.
	setFrameSize(double2(63, 180));
}

Tower * ToolsWindow::getTower()
{
	return ui->getTower();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

void ToolsWindow::draw(rectd dirtyRect)
{
	//Fill the area of the window with some debugging color
	Engine::TexturedQuad quad;
	quad.rect.size = getFrameSize();
	quad.color = (color4d){1, 0, 0, 1};
	quad.draw();
	
	//Call the original implementation of this method which will also draw the subviews
	GUI::View::draw(dirtyRect);
}
