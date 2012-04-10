#include "FastFood.h"

using namespace OT::Item;


void FastFood::init()
{
	Item::init();
	
	LOG(DEBUG, "initializing fast food");
	sf::Sprite * s = new sf::Sprite;
	s->SetImage(App->bitmaps["simtower/fastfood"]);
	s->SetSubRect(sf::IntRect(128*2, 24*0, 128*3, 24*1));
	s->Resize(128, 24);
	s->SetCenter(0, 24);
	addSprite(s);
}
