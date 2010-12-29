#include "toolboxwindow.h"

using namespace OSS;


void ToolboxWindow::eventPrepare()
{
	setFrame(recti(50, 50, 63, 180));
	backgroundSprite.texture = Texture::named("simtower/003.bmp");
}
