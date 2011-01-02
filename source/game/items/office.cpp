#include "office.h"
#include "../tower.h"

using namespace OSS;


Item::Descriptor OfficeItem::descriptor = {
	Item::kOfficeType,
	Item::kOfficeGroup,
	Item::kFacilityCategory,
	1,
	(Item::kNotBelowGroundAttribute),
	40000,
	int2(9, 1),
	int2(9, 1)
};


OfficeItem::OfficeItem() : Item()
{
	type = randui(0, 6);
	vacant = false;
	updateBackground();
}

void OfficeItem::updateBackground()
{
	//Load the appropriate office texture
	if (vacant) {
		backgroundSprite->texture = Texture::named("simtower/facilities/office/vacant");
		backgroundSprite->textureRect.size.x = 0.5;
		backgroundSprite->textureRect.origin.x = 0;
	} else {
		char str[256];
		sprintf(str, "simtower/facilities/office/inhabited/%i", type / 2);
		backgroundSprite->texture = Texture::named(str);
		backgroundSprite->textureRect.size.x = 0.25;
		backgroundSprite->textureRect.origin.x = (type % 2) * 0.5;
	}
	
	//Choose between day and night texture
	if (tower && (tower->time < 7 || tower->time >= 17))
		backgroundSprite->textureRect.origin.x += backgroundSprite->textureRect.size.x;
}

void OfficeItem::advance(double dt)
{
	Item::advance(dt);
	if (tower && (tower->checkTime(7) || tower->checkTime(17)))
		updateBackground();
}
