#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include "GameObject.h"

namespace OT {
	
	class Sky : public GameObject, public sf::Drawable
	{
	public:
		Sky(Game * game) : GameObject(game) {
			from     = 0;
			to       = 0;
			progress = 0;
			rainyDay = false;
			rainAnimation = 0;
		}
		
		int from;
		int to;
		double progress;
		
		bool rainyDay;
		double rainAnimation;
		
		void advance(double dt);
		virtual void Render(sf::RenderTarget & target) const;
	};
}
