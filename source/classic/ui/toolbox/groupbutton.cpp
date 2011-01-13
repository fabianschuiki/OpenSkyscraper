#include "groupbutton.h"

#include "window.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

ToolboxGroupButton::ToolboxGroupButton(ToolsWindow * window, ItemGroup group) :
window(window), group(group)
{
	//The group buttons have a fixed width, so set it.
	setFrameSize(double2(32, 32));
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

void ToolboxGroupButton::draw(rectd dirtyRect)
{
	Engine::TexturedQuad quad;
	quad.rect.size = getFrameSize();
	quad.color = (color4d){0, 1, 0, 1};
	quad.draw();
	
	GUI::View::draw(dirtyRect);
}
