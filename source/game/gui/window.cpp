#include "window.h"

using namespace OSS;


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
}
