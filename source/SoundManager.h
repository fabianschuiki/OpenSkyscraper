#pragma once
#include <string>
#include <SFML/Audio/SoundBuffer.hpp>
#include "ResourceManager.h"

namespace OT
{
	class Application;
	
	class SoundManager : public ResourceManager<sf::SoundBuffer>
	{
	public:
		SoundManager(Application * app) : ResourceManager<sf::SoundBuffer>(app), internal_path("simtower") { internal_path.push_back(Path::SEPARATOR); }
		bool load(Path name, sf::SoundBuffer & dst);
	private:
		std::string internal_path;
	};
}
