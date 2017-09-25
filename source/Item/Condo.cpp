#include "../Game.h"
#include "../Math/Rand.h"
#include "Condo.h"

using namespace OT;
using namespace OT::Item;


Condo::~Condo()
{
}

void Condo::init()
{
	Item::init();

	variant = rand() % 3;
	occupied = false;
	updateLighting(game->time.getHour());
	rent = 5000;
	rentDeposit = rent;

	sprite.SetImage(App->bitmaps["simtower/condo"]);
	sprite.setOrigin(0, 24);
	sprite.setPosition(getPosition().x * 8, -getPosition().y * 36);
	addSprite(&sprite);
	spriteNeedsUpdate = false;

	updateSprite();
}

void Condo::encodeXML(tinyxml2::XMLPrinter & xml)
{
	Item::encodeXML(xml);
	xml.PushAttribute("rent", rent);
	xml.PushAttribute("rentDeposit", rentDeposit);
	xml.PushAttribute("variant", variant);
	xml.PushAttribute("lighting", lighting);
	xml.PushAttribute("occupied", occupied);
}

void Condo::decodeXML(tinyxml2::XMLElement & xml)
{
	Item::decodeXML(xml);
	rent        = xml.IntAttribute("rent");
	rentDeposit = xml.IntAttribute("rentDeposit");
	variant     = xml.IntAttribute("variant");
	lighting    = (LightingConditions)xml.IntAttribute("lighting");
	occupied    = xml.BoolAttribute("occupied");
	updateSprite();
}

void Condo::updateSprite()
{
	spriteNeedsUpdate = false;
	int index = 2;
	if (occupied)
	{
		switch (lighting)
		{
		case NIGHT:
			index = 2;
			break;
		case LIT:
			index = 1;
			break;
		case DAYTIME:
			index = 0;
			break;
		}
	}
	else
	{
		switch (lighting)
		{
		case NIGHT:
		case LIT:
			index = 4;
			break;
		case DAYTIME:
			index = 3;
			break;
		}
	}

	LOG(DEBUG, "Updating sprite to index %d and variant %d", index, variant);
	sprite.setTextureRect(sf::IntRect(index*128, variant*24, 128, 24));
}

void Condo::advance(double dt)
{
	// Occupy the condo if it is attractive enough.
	if (!occupied && game->time.day != 2 && game->time.hour >= 7 && game->time.hour < 17 && isAttractive()) {
		// The item has a 10% chance of being occupied every 1/500th of a day. This means
		// that on average, an item is occupied after 1/250th of a day.
		if (game->time.checkTick(0.002) && (rand() % 10) == 0) {
			occupied = true;
			variant = rand() % 3;
			spriteNeedsUpdate = true;
			rentDeposit = rent;
			//game->populationNeedsUpdate = true;
			game->transferFunds(rentDeposit, "Occupied Condo's rent deposit");
			population = (rand() % 4) + 1;
			game->populationNeedsUpdate = true;
		}
	}

	// Monday 5:00 needs special treatment as this is where rent will be paid and tenants vacate
	// unattractive items.
	if (occupied && game->time.checkHour(5) && game->time.day == 0) {
		// Vacate unattractive offices.
		if (!isAttractive()) {
			occupied = false;
			spriteNeedsUpdate = true;
			population = 0;
			game->populationNeedsUpdate = true;
			game->transferFunds(-rentDeposit, "Vacated Condo's rent deposit payed back");
		}
		//Pay rent for the others.
		else {
			game->transferFunds(rent, "Income from Condo's rent");
		}
	}

	if (updateLighting(game->time.getHour())) {
		spriteNeedsUpdate = true;
	}

	if (spriteNeedsUpdate) updateSprite();
}

bool Condo::updateLighting(double time)
{
	LightingConditions newLighting;
	if ((time < 7.0) || (time > 22.0)) {
		newLighting = NIGHT;
	}
	else if (time < 19.0) {
		newLighting = DAYTIME;
	}
	else {
		newLighting = LIT;
	}

	if (newLighting != lighting) {
		lighting = newLighting;
	}
	else {
		return false;
	}
}

void Condo::addPerson(Person * p)
{
	Item::addPerson(p);
	spriteNeedsUpdate = true;
}

void Condo::removePerson(Person * p)
{
	Item::removePerson(p);
	spriteNeedsUpdate = true;
}

/** Returns whether the item will be vacated at the next month. */
bool Condo::isAttractive()
{
	return !lobbyRoute.empty();
}
