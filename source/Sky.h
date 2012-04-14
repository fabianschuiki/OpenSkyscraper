#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include "Math/Vector2D.h"
#include "GameObject.h"
#include <SFML/Audio/Sound.hpp>

namespace OT {
	
	class Sky : public GameObject, public sf::Drawable
	{
	public:
		Sky(Game * game);
		
		int from;
		int to;
		double progress;
		
		bool rainyDay;
		double rainAnimation;
		
		void advance(double dt);
		virtual void Render(sf::RenderTarget & target) const;
		
		static double cloudNoise(double2 p);
		
		sf::Sound rainSound;
		sf::Sound thunderSound;
		sf::Sound birdsSound;
		sf::Sound cricketsSound;
		double soundCountdown;
	};
}
