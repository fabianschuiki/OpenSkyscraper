#include <cassert>
#include "../Game.h"
#include "Lobby.h"

using namespace OT;
using namespace Item;


Lobby::~Lobby()
{
	assert(position.y != 0);
}

void Lobby::init()
{
	Item::init();
	bool groundLobby = (position.y == 0);
	
	Path p = (groundLobby ? "simtower/lobby/normal" : "simtower/lobby/sky");
	
	background.SetImage(App->bitmaps[p]);
	background.SetCenter(0, 36);
	overlay = background;
	
	if (groundLobby) {
		assert(game->mainLobby == NULL);
		game->mainLobby = this;
		
		for (int i = 0; i < 2; i++) {
			entrances[i].SetImage(App->bitmaps["simtower/deco/entrances"]);
			entrances[i].SetCenter(56*(1-i), 36);
			entrances[i].SetSubRect(sf::IntRect(i*56, 0, (i+1)*56, 36));
			addSprite(&entrances[i]);
		}
	}
	
	updateSprite();
}

void Lobby::encodeXML(tinyxml2::XMLPrinter & xml)
{
	Item::encodeXML(xml);
	xml.PushAttribute("width", size.x);
}

void Lobby::decodeXML(tinyxml2::XMLElement & xml)
{
	Item::decodeXML(xml);
	size.x = xml.IntAttribute("width");
	updateSprite();
}

void Lobby::updateSprite()
{
	int y = 0;
	if (game->rating == 2) y = 1;
	if (game->rating >= 3) y = 2;
	
	sf::IntRect rect(0, y*36, 0, (y+1)*36);
	rect.Right = 7*8;
	overlay.SetSubRect(rect);
	rect.Left  = rect.Right;
	rect.Right = 312;
	background.SetSubRect(rect);
	
	entrances[0].SetX(0);
	entrances[1].SetX(GetSize().x);
}

void Lobby::Render(sf::RenderTarget & target) const
{
	Item::Render(target);
	
	Sprite o = overlay;
	target.Draw(o);
	
	/*for (SpriteSet::iterator s = sprites.begin(); s != sprites.end(); s++) {
		game->drawnSprites++;
		target.Draw(**s);
	}*/
}
