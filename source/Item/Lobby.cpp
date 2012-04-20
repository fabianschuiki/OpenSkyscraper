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
	sf::FloatRect view = target.GetView().GetRect();
	recti rect = getRect();
	
	int minx = std::max<int>(floor(view.Left / 256), floor(rect.minX() / 32.0));
	int maxx = std::min<int>(ceil(view.Right / 256), ceil (rect.maxX() / 32.0));
	Sprite b = background;
	for (int x = minx; x < maxx; x++) {
		int offl = std::max<int>(0, rect.minX() - x*32) * 8;
		int offr = std::max<int>(0, (x+1)*32 - rect.maxX()) * 8;
		sf::IntRect sr = background.GetSubRect();
		sr.Left  += offl;
		sr.Right -= offr;
		b.SetSubRect(sr);
		
		b.SetX((x * 32 - rect.minX()) * 8 + offl);
		target.Draw(b);
		
		game->drawnSprites++;
	}
	
	Sprite o = overlay;
	sf::IntRect sr = o.GetSubRect();
	sr.Right = std::min<int>(sr.Right, GetSize().x - 16);
	o.SetSubRect(sr);
	target.Draw(o);
	game->drawnSprites++;
}
