#pragma once

namespace OT {
	class Application;
	class Game;
	
	class GameObject
	{
	public:
		Application * const app;
		Game * const game;
		GameObject(Game * game);
	};
}
