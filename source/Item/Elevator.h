#pragma once
#include "Item.h"

namespace OT {
	namespace Item {
		
		class Elevator : public Item
		{
		public:
			Elevator(Game * game, OT::Item::AbstractPrototype * prototype) : Item(game, prototype) {}
			
			virtual void init();
			
			std::string shaftBitmap;
			std::string carBitmap;
			
			double animation;
			int frame;
			
			Sprite shaft;
			Sprite topMotor;
			Sprite bottomMotor;
			
			void updateSprite();
			virtual void Render(sf::RenderTarget & target) const;
			virtual void advance(double dt);
			
			virtual void encodeXML(tinyxml2::XMLPrinter& xml);
			virtual void decodeXML(tinyxml2::XMLElement& xml);
		};
	}
}
