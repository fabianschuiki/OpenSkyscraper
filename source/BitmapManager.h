#pragma once
#include <string>
#include <SFML/Graphics/Image.hpp>
#include "ResourceManager.h"

namespace OT
{
	class Application;
	
	class BitmapManager : public ResourceManager<sf::Image>
	{
	public:
		BitmapManager(Application * app) : ResourceManager<sf::Image>(app), internal_path("simtower") { internal_path.push_back(Path::SEPARATOR); }
		bool load(Path name, sf::Image & dst);
	private:
		std::string internal_path;
	};
}
