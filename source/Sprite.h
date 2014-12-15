#pragma once
#include <SFML/Graphics.hpp>

namespace OT
{
	class Sprite : public sf::Sprite
	{
	public:
		void SetImage(sf::Image& image);
	protected:
		virtual void Render(sf::RenderTarget & target) const;

		sf::Texture texture;
	};
}
