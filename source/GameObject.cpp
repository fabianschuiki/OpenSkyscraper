#include "Application.h"
#include "Game.h"
#include "GameObject.h"

using namespace OT;


GameObject::GameObject(Game * game)
:	game(game),
	app(&game->app)
{
}
