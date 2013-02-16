/* Copyright © 2013 Fabian Schuiki */
#include "Office.h"
#include "../Game.h"
#include "../Math/Rand.h"
#include "../Time.h"

using OT::Item::Office;
using OT::Path;
using OT::Math::randd;
using OT::Time;


Office::~Office()
{
	// Get rid of the workers.
	for (Workers::iterator c = workers.begin(); c != workers.end(); c++)
		delete *c;
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

	// Create the workers.
	Person::Type types[] = {
		Person::kSalesman,
		Person::kSalesman,
		Person::kMan,
		Person::kMan,
		Person::kWoman1,
		Person::kWoman2
	};
	for (int i = 0; i < 6; i++) {
		workers.insert(new Worker(this, types[i]));
	}
	rescheduleWorkers();
	
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
			population = workers.size();
			game->populationNeedsUpdate = true;
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
			population = 0;
			game->populationNeedsUpdate = true;
			game->transferFunds(-rentDeposit, "Vacated Office's rent deposit payed back");
		}

		//Pay rent for the others.
		else {
			game->transferFunds(rent, "Income from Office rent");
		}
	}

	// Reset worker schedules at 5:00 if the office is occupied.
	if (occupied && game->time.checkHour(5) && game->time.day != 2) {
		rescheduleWorkers();
	}

	// Animate workers if occupied.
	if (occupied) {
		// Make workers arrive.
		while (!arrivalQueue.empty()) {
			Worker * c = arrivalQueue.top();
			if (game->time.hour > c->arrivalTime && !lobbyRoute.empty()) {
				arrivalQueue.pop();
				c->journey.set(lobbyRoute);
			} else break;
		}

		// Make workers leave.
		while (!departureQueue.empty()) {
			Worker * c = departureQueue.top();
			if (game->time.hour > c->departureTime) {
				departureQueue.pop();
				// Find a way home for the worker.
				const Route &r = game->findRoute(this, game->mainLobby);
				if (r.empty()) {
					LOG(DEBUG, "%p has no route to leave", c);
				} else {
					LOG(DEBUG, "%p leaving office", c);
					c->journey.set(r);
				}
			} else break;
		}

		// Make salesmen leave.
		while (!salesLeaveQueue.empty()) {
			Worker *w = salesLeaveQueue.top();
			if (game->time.check(w->leaveForSalesTime) && !lobbyRoute.empty()) {
				salesLeaveQueue.pop();
				const Route &r = game->findRoute(this, game->mainLobby);
				if (r.empty()) {
					LOG(DEBUG, "%p has no route to leave", w);
				} else {
					LOG(DEBUG, "Salesman %p leaving office", w);
					w->journey.set(r);
				}
			} else break;
		}

		// Make salesmen return.
		while (!salesReturnQueue.empty()) {
			Worker *w = salesReturnQueue.top();
			if (game->time.check(w->returnFromSalesTime) && !lobbyRoute.empty()) {
				salesReturnQueue.pop();
				LOG(DEBUG, "Calling back salesman %p", w);
				w->journey.set(lobbyRoute);
			} else break;
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

void Office::addPerson(Person * p)
{
	Item::addPerson(p);

	//Reduce the person's stress a bit, just for the time being.
	p->stress *= 0.5;

	// If this was a salesman, set a sales leave and return time for him.
	if (p->type == Person::kSalesman) {
		Worker *w = (Worker *)p;
		if (w->leaveForSalesTime < 0) {
			w->leaveForSalesTime = game->time.absolute + randd(0.01, 0.02);
			LOG(DEBUG, "Salesman %p will leave at %f", w, w->leaveForSalesTime);
			salesLeaveQueue.push(w);
		}
		if (w->returnFromSalesTime < 0) {
			w->returnFromSalesTime = floor(game->time.absolute) + randd(Time::hourToAbsolute(13), Time::hourToAbsolute(15));
			LOG(DEBUG, "Salesman %p will return at %f", w, w->returnFromSalesTime);
			salesReturnQueue.push(w);
		}
	}
}

Path Office::getRandomBackgroundSoundPath()
{
	if (!lit || !occupied) return "";
	return "simtower/office";
}

/**
 * Shuffles the schedule of all office workers for the current day. Also sets
 * up all the queues appropriately.
 */
void Office::rescheduleWorkers()
{
	LOG(DEBUG, "Rescheduling workers.");

	// Clear the current queues.
	while (!arrivalQueue.empty()) arrivalQueue.pop();
	while (!departureQueue.empty()) departureQueue.pop();
	while (!lunchQueue.empty()) lunchQueue.pop();
	while (!salesLeaveQueue.empty()) salesLeaveQueue.pop();
	while (!salesReturnQueue.empty()) salesReturnQueue.pop();

	//For each worker, decide on an arrival, departure and lunch time.
	for (Workers::iterator iw = workers.begin(); iw != workers.end(); iw++) {
		Worker *w = *iw;
		w->arrivalTime = randd(7, 8);
		w->departureTime = randd(17, 19);
		w->lunchTime = randd(12, 12.2);
		w->stress = 0;
		w->leaveForSalesTime = -1;
		w->returnFromSalesTime = -1;

		arrivalQueue.push(w);
		departureQueue.push(w);
		if (w->type != Person::kSalesman)
			lunchQueue.push(w);
	}
}
