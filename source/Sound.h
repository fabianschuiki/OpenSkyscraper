#pragma once
#include <SFML/Audio/Sound.hpp>

namespace OT {
	class Game;
	
	class Sound : public sf::Sound
	{
	public:
		Sound();
		Game * playingInGame;
		virtual void Play(Game * game);
		virtual void Stop();
	};
}
