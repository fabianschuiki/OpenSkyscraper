#include <cassert>
#include "Game.h"
#include "Sound.h"

using namespace OT;


Sound::Sound()
{
	playingInGame = NULL;
}

void Sound::Play(Game * game)
{
	assert(game);
	playingInGame = game;
	game->playingSounds.insert(this);
	sf::Sound::Play();
}

void Sound::Stop()
{
	sf::Sound::Stop();
	if (playingInGame) {
		playingInGame->playingSounds.erase(this);
		playingInGame = NULL;
	}
}

double Sound::getDurationDouble()
{
	sf::Time t = getBuffer()->getDuration();
	return t.asSeconds + 0.001*t.asMilliseconds() + 0.000001*t.asMicroseconds();
}
