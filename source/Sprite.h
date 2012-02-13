#pragma once
#include <SFML/Graphics.hpp>

namespace OT
{
	class Sprite : public sf::Sprite
	{
	public:
		int layer;
	};
}
