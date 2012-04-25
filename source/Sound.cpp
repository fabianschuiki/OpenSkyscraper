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
