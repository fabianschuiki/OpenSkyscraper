#include "button.h"

using namespace OSS;
using namespace Classic;


Button::Button()
{
	pressed = false;
}

void Button::draw(rectd dirtyRect)
{
	TexturedQuad quad;
	quad.rect.size = getFrameSize();
	quad.texture = (pressed ? pressedTexture : normalTexture);
	quad.textureRect = (pressed ? pressedTextureRect : normalTextureRect);
	quad.draw();
	
	View::draw(dirtyRect);
}
