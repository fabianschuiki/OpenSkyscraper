#include "window.h"

#include "../game.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

ControlWindow::ControlWindow(GameUI * ui) : ui(ui)
{
	//Since the control window is fixed in size, we may set the frame size right from the beginning.
	setFrameSize(double2(431, 41));
	
	//Fetch the background texture of the control window
	backgroundTexture = Texture::named("simtower/ui/control/background");
	
	//Calculate the watch's rect
	rectd watchRect(double2(), getFrameSize());
	watchRect.inset(double2(6, 6));
	watchRect.size.x = watchRect.size.y;
	
	//Initialize the watch view
	watchView = new WatchView(this);
	watchView->setFrame(watchRect);
	addSubview(watchView);
	
	//Initialize the rating view
	ratingView = new RatingView(this);
	ratingView->setFrameOrigin(double2(45, 17));
	addSubview(ratingView);
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
	TexturedQuad quad;
	quad.texture = backgroundTexture;
	quad.rect.size = getFrameSize();
	
	//Draw the background quad
	quad.draw();
	
	//Call the original implementation of this method which will also draw the subviews
	View::draw(dirtyRect);
}
