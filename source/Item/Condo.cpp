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
			game->transferFunds(rentDeposit, "Occupied Condo's rent deposit");
			createOccupants();
		}
	}

	// Monday 5:00 needs special treatment as this is where rent will be paid and tenants vacate
	// unattractive items.
	if (occupied && game->time.checkHour(5) && game->time.day == 0) {
		// Vacate unattractive offices.
		if (!isAttractive()) {
			occupied = false;
			removeOccupants();
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

	if (occupied && game->time.checkHour(3)) {
		generateJitters();
	}

	if (occupied)
	{
		moveOccupants();
	}

	if (updateLighting(game->time.getHour())) {
		spriteNeedsUpdate = true;
	}

	if (spriteNeedsUpdate) updateSprite();
}

void Condo::generateJitters()
{
	// Clear the current queues.
	while (!returnQueue.empty()) returnQueue.pop();
	while (!departureQueue.empty()) departureQueue.pop();

	for (CondoOccupant* person : occupants)
	{
		// It's life. You're more likely to be late than early.
		person->departureJitter = Math::randd(-0.1, 0.3);
		person->returnJitter = Math::randd(-0.1, 0.3);
		returnQueue.push(person);
		departureQueue.push(person);
	}
}

void Condo::moveOccupants()
{
	// Occupants leave the building
	while (!departureQueue.empty()) {
		CondoOccupant * c = departureQueue.top();
		if (game->time.hour > c->actualDepartureTime()) {
			departureQueue.pop();
			c->journey.set(lobbyRoute);
		} else break;
	}

	//Occupants return from their busy days
	while (!returnQueue.empty()) {
		CondoOccupant * c = returnQueue.top();
		if (game->time.hour > c->actualReturnTime() && !lobbyRoute.empty()) {
			returnQueue.pop();
			// Find a way home for the Person.
			const Route &r = game->findRoute(this, game->mainLobby);
			if (r.empty()) {
				LOG(DEBUG, "%p has no route to leave his or her Condo", c);
			} else {
				LOG(DEBUG, "%p leaving condo", c);
				c->journey.set(r);
			}
		} else break;
	}
}

bool Condo::updateLighting(double time)
{
	LightingConditions newLighting = lighting;
	if ((time < 7.0) || (time > 22.0)) {
		newLighting = NIGHT;
	}
	else if (time < 19.0) {
		newLighting = DAYTIME;
	}
	else {
		newLighting = LIT;
	}

	bool retval = (newLighting != lighting);
	lighting = newLighting;
	return retval;
}

void Condo::addPerson(Person * p)
{
	Item::addPerson(p);
	spriteNeedsUpdate = true;
}

void Condo::createOccupants()
{
	// Each Condo must have at least one adult
	unsigned numAdults = Math::randi(1, 4);
	Person::Type adults[] = {
		Person::kMan,
		Person::kWoman1,
		Person::kWoman2
	};

	for (int i = 0; i < numAdults; ++i)
	{
		// Bias towards man because there are 2 women types and one man.
		// from the rand function, both 0 and 1 mean man, with the other two being the two woman types
		unsigned gender = std::max(Math::randi(0, 3) - 1, 0);

		// Everyone works or goes to school, which means no activity during the day,
		// which could be made more realistic by having people who do chores during the day,
		// such as stay at home Moms or Dads.
		double leavingTime = Math::randd(7.5, 9.5);
		double returnTime = Math::randd(17.5, 19.5);

		occupants.insert(new CondoOccupant(this, adults[gender], leavingTime, returnTime));
	}

	// We generously assume that the kids will never more than double outnumber the number of adults,
	// and that the total occupancy will not be more than 6
	unsigned numKids = Math::randi(0, std::min(numAdults * 2, 6u));

	Person::Type kids[] = {
		Person::kChild,
		Person::kWomanWithChild1,
		Person::kWomanWithChild2,
	};

	for (int i = 0; i < numKids; ++i)
	{
		unsigned type = Math::randi(0, 2);
		// All schools begin and end at the same time.
		// Some kids have after school care though.
		double leavingTime = 7.5;
		double returnTime = (Math::randi(0, 1) == 0) ? 15.5 : 17.5;

		occupants.insert(new CondoOccupant(this, kids[type], leavingTime, returnTime));
	}

	population = occupants.size();
	game->populationNeedsUpdate = true;
}

void Condo::removeOccupants()
{
	for (Person* occupant : occupants)
	{
		removePerson(occupant);
		delete occupant;
	}
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

double Condo::CondoOccupant::actualReturnTime() const
{
	return departureTime + departureJitter;
}

double Condo::CondoOccupant::actualDepartureTime() const
{
	return returnTime + returnJitter;
}
