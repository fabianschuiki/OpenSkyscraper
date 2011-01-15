#include "window.h"

using namespace OSS;


void Window::draw(rectd dirtyRect)
{
	rectd rect;
	rect.size = getFrameSize();
	rect.inset(double2(-0.5, -0.5));
	
	glColor4f(0, 0, 0, 0.75);
	Texture::unbind();
	glBegin(GL_LINE_STRIP);
	glVertex2d(rect.minX(), rect.minY());
	glVertex2d(rect.minX(), rect.maxY());
	glVertex2d(rect.maxX(), rect.maxY());
	glVertex2d(rect.maxX(), rect.minY());
	glVertex2d(rect.minX(), rect.minY());
	glEnd();
	
	View::draw(dirtyRect);
}
