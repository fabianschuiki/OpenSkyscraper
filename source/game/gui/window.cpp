#include "window.h"

using namespace OSS;


Window::Window()
{
	borderColor = (color4d){0, 0, 0, 1.0};
}

void Window::setFrame(recti frame)
{
	this->frame = frame;
	backgroundSprite.rect = frame;
}

recti Window::getFrame()
{
	return frame;
}

void Window::draw(rectd visibleRect)
{
	backgroundSprite.draw(visibleRect);
	
	//Draw the border
	//glPushAttrib(GL_POLYGON_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	rectd borderRect = getFrame();
	borderRect.inset(-0.5, -0.5);
	glBindTexture(GL_TEXTURE_RECTANGLE_EXT, 0);
	glColor4dv(borderColor.v);
	glBegin(GL_QUADS);
	glVertex2d(borderRect.minX(), borderRect.minY());
	glVertex2d(borderRect.maxX(), borderRect.minY());
	glVertex2d(borderRect.maxX(), borderRect.maxY());
	glVertex2d(borderRect.minX(), borderRect.maxY());
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glPopAttrib();
}

void Window::eventVideoModeChanged()
{
	layout();
}
