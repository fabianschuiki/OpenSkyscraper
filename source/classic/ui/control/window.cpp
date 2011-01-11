#include "window.h"

#include "../game.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

ControlWindow::ControlWindow(GameUI * ui) : GUI::View(), ui(ui)
{
	//Since the control window is fixed in size, we may set the frame size right from the beginning.
	setFrameSize(double2(431, 41));
	
	//Fetch the background texture of the control window
	backgroundTexture = Engine::Texture::named("simtower/ui/control/background");
}

Tower * ControlWindow::getTower()
{
	return ui->getTower();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

void ControlWindow::draw(rectd dirtyRect)
{
	//Create a textured quad and make it fill our entire frame
	Engine::TexturedQuad quad;
	quad.texture = backgroundTexture;
	quad.rect.size = getFrameSize();
	
	//Draw the background quad
	quad.draw();
	
	//Call the original implementation of this method which will also draw the subviews
	GUI::View::draw(dirtyRect);
}
