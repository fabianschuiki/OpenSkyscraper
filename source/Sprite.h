#pragma once
#include <SFML/Graphics.hpp>

namespace OT
{
	class Sprite : public sf::Sprite
	{
	protected:
		virtual void Render(sf::RenderTarget & target) const;
	};
}
