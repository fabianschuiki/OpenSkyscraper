#include "Sprite.h"
#include <GL/gl.h>

using namespace OT;


void Sprite::Render(sf::RenderTarget & target) const
{
	glTranslatef(-0.375f, -0.375f, 0.f);
	sf::Sprite::Render(target);
}		
