#pragma once

namespace OT {
	class Game;
	
	class GameObject
	{
	public:
		Game * const game;
		GameObject(Game * game) : game(game) {}
	};
}