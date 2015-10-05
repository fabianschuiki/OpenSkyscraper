#pragma once
#include <SFML/Graphics.hpp>

namespace OT
{
	class Sprite : public sf::Sprite
	{
	public:
		void SetImage(const sf::Image& image);
		void SetImage(const sf::Texture& texture);
		sf::Vector2u getSize() const { return sf::Vector2u(getTextureRect().width, getTextureRect().height); }
	protected:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		sf::Texture texture;
	};
}
