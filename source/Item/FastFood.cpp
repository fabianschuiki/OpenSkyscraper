#include "../Game.h"
#include "../Math/Rand.h"
#include "FastFood.h"

using namespace OT;
using namespace OT::Item;


FastFood::~FastFood()
{
	clearCustomers();
}

void FastFood::init()
{
	Item::init();
	
	variant = rand() % 5;
	open = false;
	
	sprite.SetImage(App->bitmaps["simtower/fastfood"]);
	sprite.SetCenter(0, 24);
	addSprite(&sprite);
	spriteNeedsUpdate = false;
	
	defaultCeiling();
	
	updateSprite();
}

void FastFood::encodeXML(tinyxml2::XMLPrinter & xml)
{
	Item::encodeXML(xml);
	xml.PushAttribute("variant", variant);
	xml.PushAttribute("open", open);
}

void FastFood::decodeXML(tinyxml2::XMLElement & xml)
{
	Item::decodeXML(xml);
	variant = xml.IntAttribute("variant");
	open    = xml.BoolAttribute("open");
	updateSprite();
}

void FastFood::updateSprite()
{
	spriteNeedsUpdate = false;
	int index = 3;
	if (open) index = std::min<int>(ceil(people.size() / 5.0), 2);
	sprite.SetSubRect(sf::IntRect(index*128, variant*24, (index+1)*128, (variant+1)*24));
	sprite.Resize(128, 24);
}

void FastFood::advance(double dt)
{
	//Open
	if (game->time.checkHour(10)) {
		open = true;
		spriteNeedsUpdate = true;
		
		//Create new customers for today.
		int today = 10;
		clearCustomers();
		for (int i = 0; i < today; i++) {
			Customer * c = new Customer(this);
			c->arrivalTime = (game->time.year - 1) * 12 + (game->time.quarter - 1) * 3 + game->time.day + Math::randd(Time::hourToAbsolute(10), Time::hourToAbsolute(20));
			customers.insert(c);
		}
	}
	
	//Close
	if (game->time.checkHour(21) && open) {
		open = false;
		population = customerMetadata.size();
		game->populationNeedsUpdate = true;
		spriteNeedsUpdate = true;
		
		game->transferFunds(population * 200 - 2000, "Income from Fast Food");
	}
	
	//Make customers arrive.
	for (Customers::iterator i = customers.begin(); i != customers.end(); i++) {
		Customer * c = *i;
		if (game->time.check(c->arrivalTime)) c->journey.set(lobbyRoute);
	}
	
	//Make customers leave once they're done.
	for (CustomerMetadataMap::iterator i = customerMetadata.begin(); i != customerMetadata.end();) {
		Person *p = i->first;
		CustomerMetadata &m = i->second;
		i++;
		if (m.departureTime == 0 && (game->time.absolute >= m.arrivalTime + 20 * Time::kBaseSpeed || game->time.hour >= 21)) {
			//TODO: actually make the person journey away.
			LOG(DEBUG, "%p leaving", p);
			m.departureTime = game->time.absolute;
			removePerson(p);
		}
	}
	
	if (spriteNeedsUpdate) updateSprite();
}

void FastFood::addPerson(Person * p)
{
	Item::addPerson(p);
	CustomerMetadata & m = customerMetadata[p];
	m.arrivalTime = game->time.absolute;
	m.departureTime = 0;
	spriteNeedsUpdate = true;
}

void FastFood::removePerson(Person * p)
{
	Item::removePerson(p);
	spriteNeedsUpdate = true;
}

void FastFood::clearCustomers()
{
	for (Customers::iterator c = customers.begin(); c != customers.end(); c++)
		delete *c;
	customers.clear();
	customerMetadata.clear();
}

FastFood::Customer::Customer(FastFood * item)
:	Person(item->game)
{
	arrivalTime = 0;
	Type types[] = {kMan, kWoman1, kWoman2, kWomanWithChild1};
	type = types[rand() % 4];
}
