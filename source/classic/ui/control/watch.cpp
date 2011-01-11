#include "watch.h"

#include "window.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

Watch::Watch(ControlWindow * window) : GUI::View(), controlWindow(window)
{
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

void Watch::draw(rectd dirtyRect)
{
	//Get the tower from our control window parent
	Tower * tower = controlWindow->getTower();
	if (!tower)
		return;
	
	//Calculate the center and radius of the watch
	double2 center = getFrameSize() / 2;
	double radius = mind(center.x, center.y);
	
	//Get the time of day from the tower
	double tod = tower->time->getTimeOfDay();
	
	//Calculate the rotation angles of the hour and minute hands. The hour hand advances 12 times
	//slower than the minute hand.
	double hourHandAngle = 2 * M_PI * (tod / 12);
	double minuteHandAngle = 2 * M_PI * (tod);
	
	//Calculate the tip positions of the hands
	double2 hourTip(sin(hourHandAngle), cos(hourHandAngle));
	double2 minuteTip(sin(minuteHandAngle), cos(minuteHandAngle));
	hourTip *= radius * 0.6;
	hourTip += center;
	minuteTip *= radius;
	minuteTip += center;
	
	//Draw the hands
	glColor4f(0, 0, 0, 1);
	Engine::Texture::unbind();
	glBegin(GL_LINES);
	glVertex2f(center.x, center.y);
	glVertex2f(hourTip.x, hourTip.y);
	glVertex2f(center.x, center.y);
	glVertex2f(minuteTip.x, minuteTip.y);
	glEnd();
	
}
