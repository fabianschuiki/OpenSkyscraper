#include "controlwindow.h"
#include "../../core/openglcanvas.h"

using namespace OSS;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

ControlWindow::ControlWindow() : Window()
{
	//Initialize the background sprite
	backgroundSprite.texture = Texture::named("simtower/ui/control/background");
	
	//Initialize the tower rating sprite
	towerRatingSprite.texture = Texture::named("simtower/ui/control/towerrating");
	towerRatingSprite.rect.size = int2(108, 22);
	
	//Initialize the star sprites
	for (int i = 0; i < 5; i++)
		starSprite[i].rect.size = int2(24, 19);
	
	//Initial layout
	layout();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Layout
//----------------------------------------------------------------------------------------------------

void ControlWindow::layout()
{
	//Layout the window
	recti frame;
	frame.origin = OpenGLCanvas::shared()->currentMode.resolution;
	frame.size = int2(431, 41);
	frame.origin -= frame.size;
	setFrame(frame);
	
	//Layout rating sprites
	int2 ratingOrigin = (frame.origin + int2(44, 19));
	for (int i = 0; i < 5; i++)
		starSprite[i].rect.origin = (ratingOrigin + int2(i * 21, 0));
	towerRatingSprite.rect.origin = ratingOrigin - int2(1, 2);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Rendering
//----------------------------------------------------------------------------------------------------

void ControlWindow::draw(rectd visibleRect)
{
	Window::draw(visibleRect);
	drawWatch(visibleRect);
	drawRating(visibleRect);
}

void ControlWindow::drawWatch(rectd visibleRect)
{
	if (!tower) return;
	
	//Calculate the watch rect
	rectd watchRect = getFrame();
	watchRect.inset(6, 6);
	watchRect.size.x = watchRect.size.y;
	
	//Calculate the center of the watch
	double2 watchCenter = watchRect.center();
	double watchRadius = watchRect.size.x / 2;
	
	//Calculate the rotation angles of the hour and minute hands
	double hourHandAngle = 2 * M_PI * (tower->time->getTime() / 12);
	double minuteHandAngle = 2 * M_PI * (tower->time->getTime());
	
	//Calculate the tip positions of the hands
	double2 hourTip(sin(hourHandAngle), cos(hourHandAngle));
	double2 minuteTip(sin(minuteHandAngle), cos(minuteHandAngle));
	hourTip *= watchRadius * 0.6;
	minuteTip *= watchRadius;
	hourTip += watchCenter;
	minuteTip += watchCenter;
	
	//Draw the hands
	glColor4f(0, 0, 0, 1);
	glBindTexture(GL_TEXTURE_RECTANGLE_EXT, 0);
	glBegin(GL_LINES);
	glVertex2f(watchCenter.x, watchCenter.y);
	glVertex2f(hourTip.x, hourTip.y);
	glVertex2f(watchCenter.x, watchCenter.y);
	glVertex2f(minuteTip.x, minuteTip.y);
	glEnd();
}

void ControlWindow::drawRating(rectd visibleRect)
{
	if (!tower) return;
	
	//Update the star rating sprites if required
	if (visibleStarRating != tower->rating) {
		visibleStarRating = tower->rating;
		for (int i = 0; i < 5; i++)
			starSprite[i].texture = Texture::named(std::string("simtower/ui/control/star/") +
												   (i < visibleStarRating ? "filled" : "empty"));
	}
	
	//Draw either the rating stars or the tower rating
	if (visibleStarRating < 6) {
		for (int i = 0; i < 5; i++)
			starSprite[i].draw(visibleRect);
	} else {
		towerRatingSprite.draw(visibleRect);
	}
}
