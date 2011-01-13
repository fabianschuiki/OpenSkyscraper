#include "groupbutton.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

ToolsGroupButton::ToolsGroupButton(ToolsWindow * window, ItemGroup group) :
window(window), group(group)
{
	//The group buttons have a fixed width, so set it.
	setFrameSize(double2(32, 32));
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

void ToolsGroupButton::draw(rectd dirtyRect)
{
	TexturedQuad quad;
	quad.rect.size = getFrameSize();
	quad.color = (color4d){0, 1, 0, 1};
	quad.draw();
	
	View::draw(dirtyRect);
}
