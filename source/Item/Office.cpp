/* Copyright © 2013 Fabian Schuiki */
#include "Office.h"
#include "../Game.h"

using OT::Item::Office;
using OT::Path;


Office::~Office()
{
}

void Office::init()
{
	Item::init();
	
	variant = 0;
	occupied = false;
	lit = false;
	rent = 10000;
	rentDeposit = rent;

	sprite.SetImage(App->bitmaps["simtower/office"]);
	sprite.SetCenter(0, 24);
	addSprite(&sprite);
	spriteNeedsUpdate = false;
	
	defaultCeiling();
	
	updateSprite();
}

void Office::encodeXML(tinyxml2::XMLPrinter & xml)
{
	Item::encodeXML(xml);
	xml.PushAttribute("rent", rent);
	xml.PushAttribute("rentDeposit", rentDeposit);
	xml.PushAttribute("variant", variant);
	xml.PushAttribute("occupied", occupied);
	xml.PushAttribute("lit", lit);
}

void Office::decodeXML(tinyxml2::XMLElement & xml)
{
	Item::decodeXML(xml);
	rent        = xml.IntAttribute("rent");
	rentDeposit = xml.IntAttribute("rentDeposit");
	variant     = xml.IntAttribute("variant");
	occupied    = xml.BoolAttribute("occupied");
	lit         = xml.BoolAttribute("lit");
	updateSprite();
}

void Office::updateSprite()
{
	spriteNeedsUpdate = false;
	int index_x = (lit ? 0 : 1);
	int index_y = (occupied ? variant : 6);
	sprite.SetSubRect(sf::IntRect(index_x*72, index_y*24, (index_x+1)*72, (index_y+1)*24));
	sprite.Resize(72, 24);
}

void Office::advance(double dt)
{
	// Occupy the office if it is attractive enough.
	if (!occupied && game->time.day != 2 && game->time.hour >= 7 && game->time.hour < 17 && isAttractive()) {
		// The item has a 10% chance of being occupied every 1/500th of a day. This means
		// that on average, an item is occupied after 1/250th of a day.
		if (game->time.checkTick(0.002) && (rand() % 10) == 0) {
			occupied = true;
			variant = rand() % 6;
			lit = true;
			spriteNeedsUpdate = true;
			rentDeposit = rent;
			game->transferFunds(rentDeposit, "Occupied Office's rent deposit");
		}
	}

	// Monday 5:00 needs special treatment as this is where rent will be paid and tenants vacate
	// unattractive items.
	if (occupied && game->time.checkHour(5) && game->time.day == 0) {
		// Vacate unattractive offices.
		if (!isAttractive()) {
			occupied = false;
			spriteNeedsUpdate = true;
			game->transferFunds(-rentDeposit, "Vacated Office's rent deposit payed back");
		}

		//Pay rent for the others.
		else {
			game->transferFunds(rent, "Income from Office rent");
		}
	}

	//Turn on the office lights.
	bool shouldBeLit = (game->time.day != 2 && game->time.hour >= 7 && game->time.hour < 17) || !people.empty();
	if (lit != shouldBeLit ) {
		lit = shouldBeLit;
		spriteNeedsUpdate = true;
	}

	if (spriteNeedsUpdate) updateSprite();
}

/** Returns whether the item will be vacated at the next month. */
bool Office::isAttractive()
{
	return !lobbyRoute.empty();
}

Path Office::getRandomBackgroundSoundPath()
{
	if (!lit) return "";
	//char name[128];
	/*snprintf(name, 128, "simtower/Office/%i", rand() % 3);
	// Maybe we should make the coice of the sound based on the number of customers, not
	// completely random.
	return name;*/
	return "simtower/office";
}
