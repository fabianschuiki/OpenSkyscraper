#include "Stairs.h"

using namespace OT::Item;


void Stairs::init()
{
	frameCount = 14;
	sprite.SetImage(App->bitmaps["simtower/stairs"]);
	Stairlike::init();
}
