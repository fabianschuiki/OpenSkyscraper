#include "../Game.h"
#include "../Math/Rand.h"
#include "Restaurant.h"

using namespace OT;
using namespace OT::Item;


Restaurant::~Restaurant()
{
	clearCustomers();
}

void Restaurant::init()
{
	Item::init();
	
	variant = rand() % 4;
	open = false;
	
	sprite.SetImage(App->bitmaps["simtower/restaurant"]);
	sprite.SetCenter(0, 24);
	addSprite(&sprite);
	spriteNeedsUpdate = false;
	
	updateSprite();
}

void Restaurant::encodeXML(tinyxml2::XMLPrinter & xml)
{
	Item::encodeXML(xml);
	xml.PushAttribute("variant", variant);
	xml.PushAttribute("open", open);
}

void Restaurant::decodeXML(tinyxml2::XMLElement & xml)
{
	Item::decodeXML(xml);
	variant = xml.IntAttribute("variant");
	open    = xml.BoolAttribute("open");
	updateSprite();
}

void Restaurant::updateSprite()
{
	spriteNeedsUpdate = false;
	int index = 3;
	if (open) index = std::min<int>(ceil(people.size() / 5.0), 2);
	sprite.SetSubRect(sf::IntRect(index*192, variant*24, (index+1)*192, (variant+1)*24));
	sprite.Resize(192, 24);
}

void Restaurant::advance(double dt)
{
	//Open
	if (game->time.checkHour(17)) {
		open = true;
		spriteNeedsUpdate = true;
		
		//Create new customers for today.
		int today = 10;
		clearCustomers();
		for (int i = 0; i < today; i++) {
			Customer * c = new Customer(this);
			c->arrivalTime = (game->time.year - 1) * 12 + (game->time.quarter - 1) * 3 + game->time.day + Math::randd(Time::hourToAbsolute(17), Time::hourToAbsolute(22));
			customers.insert(c);
			arrivingCustomers.push(c);
		}
	}
	
	//Close
	if (game->time.checkHour(23) && open) {
		open = false;
		population = customerMetadata.size();
		game->populationNeedsUpdate = true;
		spriteNeedsUpdate = true;
		
		game->transferFunds(population * 400 - 4000, "Income from Restaurant");
	}
	
	//Make customers arrive.
	while (!arrivingCustomers.empty()) {
		Customer * c = arrivingCustomers.top();
		if (game->time.absolute > c->arrivalTime && !lobbyRoute.empty()) {
			arrivingCustomers.pop();
			c->journey.set(lobbyRoute);
		} else break;
	}
	
	//Make customers leave once they're done.
	for (std::list<Person *>::iterator ip = eatingCustomers.begin(); ip != eatingCustomers.end();) {
		Person * p = *ip;
		CustomerMetadata &m = customerMetadata[p];
		if (game->time.absolute >= m.arrivalTime + 20 * Time::kBaseSpeed || !open) {
			const Route &r = game->findRoute(this, game->mainLobby); // Customers may leave for different destinations besides main lobby, so this is not precomputed
			if (r.empty()) {
				LOG(DEBUG, "%p has no route to leave", p);
				ip++;
			} else {
				LOG(DEBUG, "%p leaving", p);
				ip = eatingCustomers.erase(ip);
				removePerson(p);
				p->journey.set(r);
			}
		} else break;
	}
	
	if (spriteNeedsUpdate) updateSprite();
}

void Restaurant::addPerson(Person * p)
{
	Item::addPerson(p);
	CustomerMetadata & m = customerMetadata[p];
	m.arrivalTime = game->time.absolute;
	eatingCustomers.push_back(p);
	spriteNeedsUpdate = true;
}

void Restaurant::removePerson(Person * p)
{
	Item::removePerson(p);
	spriteNeedsUpdate = true;
}

void Restaurant::clearCustomers()
{
	for (Customers::iterator c = customers.begin(); c != customers.end(); c++)
		delete *c;
	while (!arrivingCustomers.empty()) arrivingCustomers.pop();
	eatingCustomers.clear();
	customers.clear();
	customerMetadata.clear();
}

Restaurant::Customer::Customer(Restaurant * item)
:	Person(item->game)
{
	arrivalTime = 0;
	Type types[] = {kMan, kWoman1, kWoman2, kWomanWithChild1};
	type = types[rand() % 4];
}

Path Restaurant::getRandomBackgroundSoundPath()
{
	if (!open) return "";
	return "simtower/restaurant";
}
