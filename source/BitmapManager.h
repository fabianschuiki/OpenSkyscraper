#pragma once
#include <SFML/Graphics/Image.hpp>
#include "ResourceManager.h"

namespace OT
{
	class Application;
	
	class BitmapManager : public ResourceManager<sf::Image>
	{
	public:
		BitmapManager(Application * app) : ResourceManager<sf::Image>(app) {}
		bool load(Path name, sf::Image & dst);
	};
}
