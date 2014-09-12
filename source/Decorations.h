/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "Sprite.h"
#include "GameObject.h"
#include <map>
#include <SFML/Graphics/Drawable.hpp>

namespace OT {
	class Game;

	class Decorations : public GameObject, public sf::Drawable
	{
	public:
		Decorations(Game * game);

		void updateFloor(int y);
		void updateCrane();
		void updateTracks();

		void draw(sf::RenderTarget & target, sf::RenderStates states) const;
		virtual void Render(sf::RenderTarget & target) const;

	private:
		struct FireStairPair {
			Sprite minX;
			Sprite maxX;
		};
		typedef std::map<int, FireStairPair> FireStairs;
		FireStairs fireStairs;

		Sprite crane;
		bool craneVisible;

		Sprite track;
		bool tracksVisible;
	};
}
