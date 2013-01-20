#include <cassert>
#include <string>
#include "../Game.h"
#include "Item.h"

using namespace OT::Item;
using std::string;
using OT::int2;
using OT::Person;
using OT::rectd;
using OT::Sprite;


Item::~Item()
{
	sprites.clear();
	for (People::iterator p = people.begin(); p != people.end(); p++) removePerson(*p);
}

void Item::setPosition(int2 p)
{
	if (position != p) {
		position = p;
		SetPosition(p.x*8, -p.y*36);
	}
}

void Item::addSprite(Sprite * sprite)
{
	assert(sprite);
	sprites.insert(sprite);
}

void Item::removeSprite(Sprite * sprite)
{
	assert(sprite);
	sprites.erase(sprite);
}

void Item::Render(sf::RenderTarget & target) const
{
	for (SpriteSet::iterator s = sprites.begin(); s != sprites.end(); s++) {
		game->drawnSprites++;
		target.Draw(**s);
	}
	
	if (!canHaulPeople() && game->mainLobby != this && lobbyRoute.empty()) {
		Sprite noroute;
		noroute.SetImage(app->bitmaps["noroute.png"]);
		sf::Vector2f size = noroute.GetSize();
		noroute.SetCenter(size.x/2, size.y/2);
		size = GetSize();
		noroute.SetPosition(size.x/2, -size.y/2);
		target.Draw(noroute);
	}
}

void Item::encodeXML(tinyxml2::XMLPrinter & xml)
{
	xml.PushAttribute("type", prototype->id.c_str());
	xml.PushAttribute("x", position.x);
	xml.PushAttribute("y", position.y);
}

void Item::decodeXML(tinyxml2::XMLElement & xml)
{
}

void Item::defaultCeiling()
{
	ceiling.SetImage(App->bitmaps["simtower/floor"]);
	ceiling.SetSubRect(sf::IntRect(0, 0, 8, 12));
	ceiling.Resize(GetSize().x, 12);
	//ceiling.SetCenter(0.375, 0.375);
	ceiling.SetPosition(sf::Vector2f(0, -GetSize().y));
	addSprite(&ceiling);
}

void Item::addPerson(Person * p)
{
	assert(p);
	assert(!p->at);
	p->at = this;
	people.insert(p);
}

void Item::removePerson(Person * p)
{
	assert(p);
	assert(p->at == this);
	p->at = NULL;
	people.erase(p);
}

rectd Item::getMouseRegion()
{
	sf::Vector2f p = GetPosition();
	sf::Vector2f s = GetSize();
	return rectd(p.x, p.y - s.y + 12, s.x, s.y - 12);
}

void Item::updateRoutes()
{
	if (!canHaulPeople() && game->mainLobby && game->mainLobby != this) {
		lobbyRoute = game->findRoute(game->mainLobby, this);
	} else {
		lobbyRoute.clear();
	}
}
