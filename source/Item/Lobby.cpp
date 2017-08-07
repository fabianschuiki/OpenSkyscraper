/* Copyright (c) 2012-2015 Fabian Schuiki */
#include <cassert>
#include "../Game.h"
#include "Lobby.h"

using namespace OT;
using namespace Item;


Lobby::~Lobby() {
	assert(position.y != 0);
}

void Lobby::init() {
	Item::init();
	bool groundLobby = (position.y == 0);
	LOG(INFO, "init lobby positioned at {%d %d} by {%d %d}", position.x, position.y, size.x, size.y);

	Path p = (groundLobby ? "simtower/lobby/normal" : "simtower/lobby/sky");

	background.SetImage(App->bitmaps[p]);
	background.setOrigin(0, 36);

	overlay.SetImage(App->bitmaps[p]);
	overlay.setOrigin(0, 36);
	// overlay = background;

	if (groundLobby) {
		assert(game->mainLobby == NULL);
		game->mainLobby = this;

		for (int i = 0; i < 2; i++) {
			entrances[i].SetImage(App->bitmaps["simtower/deco/entrances"]);
			entrances[i].setOrigin(56*(1-i), 36);
			entrances[i].setTextureRect(sf::IntRect(i*56, 0, 56, 36));
			addSprite(&entrances[i]);
		}
	}

	updateSprite();
}

void Lobby::encodeXML(tinyxml2::XMLPrinter & xml) {
	Item::encodeXML(xml);
	xml.PushAttribute("width", size.x);
}

void Lobby::decodeXML(tinyxml2::XMLElement & xml) {
	Item::decodeXML(xml);
	size.x = xml.IntAttribute("width");
	updateSprite();
}

void Lobby::updateSprite() {
	int y = 0;
	if (game->rating == 2) y = 1;
	if (game->rating >= 3) y = 2;

	overlay.setTextureRect(sf::IntRect(0, y*36, 56, 36));
	background.setTextureRect(sf::IntRect(56, y*36, 256, 36));

	entrances[0].setPosition(getRect().minX()*8, 0);
	entrances[1].setPosition(getRect().maxX()*8, 0);
}

void Lobby::render(sf::RenderTarget & target) const {
	Item::render(target);

	const sf::View &view = target.getView();
	sf::Vector2f center = view.getCenter();
	sf::Vector2f size = view.getSize();
	sf::Vector2f dmax = center + size/2.f;
	sf::Vector2f dmin = center - size/2.f;

	recti rect = getRect();

	int minx = std::max<int>(floor(dmin.x / 256), floor(rect.minX() / 32.0));
	int maxx = std::min<int>(ceil (dmax.x / 256), ceil (rect.maxX() / 32.0));

	Sprite b = background;
	for (int x = minx; x < maxx; x++) {
		int cutoffLeft = 0;
		int cutoffRight = 0;

		cutoffLeft = std::max(0, rect.minX()*8 - x*256);
		cutoffRight = std::max(0, (x+1)*256 - rect.maxX()*8);

		sf::IntRect r = background.getTextureRect();
		r.left += cutoffLeft;
		r.width -= cutoffLeft + cutoffRight;

		b.setTextureRect(r);
		b.setPosition(x*256 + cutoffLeft, 0);
		target.draw(b);
		game->drawnSprites++;
	}

	Sprite o = overlay;
	sf::IntRect r = o.getTextureRect();
	r.width = std::min(r.width, rect.size.x*8-16);
	o.setTextureRect(r);
	o.setPosition(rect.minX()*8, 0);
	target.draw(o);
	game->drawnSprites++;
}
