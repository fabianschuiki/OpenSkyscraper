#include "toolboxwindow.h"

using namespace OSS;


void ToolboxWindow::eventPrepare()
{
	setFrame(recti(50, 50, 63, 180));
	backgroundSprite.texture = Texture::named("simtower/ui/toolbox/construction-N");
}
