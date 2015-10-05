#include "Sprite.h"
#include "OpenGL.h"

using namespace OT;

void Sprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	glTranslatef(-0.375f, -0.375f, 0.f);
	target.draw((const sf::Sprite&)*this, states);
}

void Sprite::SetImage(const sf::Image& image)
{
	texture.loadFromImage(image);
	setTexture(texture);
}

void Sprite::SetImage(const sf::Texture& texture)
{
	setTexture(texture);
}
