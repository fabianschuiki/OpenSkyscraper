#pragma once
#include <SFML/Audio/SoundBuffer.hpp>
#include "ResourceManager.h"

namespace OT
{
	class Application;
	
	class SoundManager : public ResourceManager<sf::SoundBuffer>
	{
	public:
		SoundManager(Application * app) : ResourceManager<sf::SoundBuffer>(app) {}
		bool load(Path name, sf::SoundBuffer & dst);
	};
}
