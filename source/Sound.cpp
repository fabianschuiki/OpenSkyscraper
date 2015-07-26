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
	sf::Sound::play();
}

void Sound::Stop()
{
	sf::Sound::stop();
	if (playingInGame) {
		playingInGame->playingSounds.erase(this);
		playingInGame = NULL;
	}
}

double Sound::getDurationDouble()
{
	return getBuffer()->getDuration().asSeconds();
}
