#include "sprite.h"

using namespace OSS;


//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

Sprite::Sprite()
{
	hidden = false;
	autoTexRectAttributes = 0;
	autoTexRectScale = double2(1, 1);
	autoTexRectRelativeTo = rectd::kCenter;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

void Sprite::draw()
{
	if (hidden) return;
	
	//Autogenerate the texture rect where necessary
	autogenerateTextureRect((autoTexRectAttributes & kAutoX), (autoTexRectAttributes & kAutoY),
							(autoTexRectAttributes & kRelative
							 ? rect.point(autoTexRectRelativeTo)
							 : double2()),
							autoTexRectScale);
	
	//Draw
	TexturedQuad::draw();
}
