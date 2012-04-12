#pragma once

#include <set>
#include <SFML/Graphics.hpp>
#include <tinyxml2.h>

#include "../GameObject.h"
#include "../Math/Rect.h"
#include "../Person.h"
#include "../Sprite.h"
#include "Prototype.h"

namespace OT {
	namespace Item {
		
		class Item : public GameObject, public sf::Drawable
		{
		public:
			int layer;
			AbstractPrototype * const prototype;
			Item(Game * game, AbstractPrototype * prototype) : GameObject(game), sf::Drawable(), prototype(prototype), size(prototype->size.x*8, prototype->size.y*36) { layer = 0; }
			virtual ~Item();
			virtual void init() {}
			
			typedef std::set<Sprite *> SpriteSet;
			SpriteSet sprites;
			void addSprite(Sprite * sprite);
			void removeSprite(Sprite * sprite);
			
			int2 position;
			const int2 size;
			void setPosition(int2 p);
			recti getRect() { return recti(position, size); }
			
			virtual void Render(sf::RenderTarget & target) const;
			sf::Vector2f GetSize() const { return sf::Vector2f(size.x, size.y); }
			
			virtual void encodeXML(tinyxml2::XMLPrinter & xml);
			virtual void decodeXML(tinyxml2::XMLElement & xml);
			
			Sprite ceiling;
			void defaultCeiling();
			
			virtual void advance(double dt) {}
			
			typedef std::set<Person *> People;
			People people;
			void addPerson(Person * p);
			void removePerson(Person * p);
		};
	}
}

#define OT_ITEM_PROTOTYPE(cls)\
	static AbstractPrototype * makePrototype() {\
		AbstractPrototype * p = new Prototype<cls>;\
		initPrototype(p);\
		return p;\
	}\
	static void initPrototype(AbstractPrototype * p)

#define OT_ITEM_CONSTRUCTOR(cls) cls(Game * game, AbstractPrototype * prototype) : Item(game, prototype) {}

#include "../Application.h"
