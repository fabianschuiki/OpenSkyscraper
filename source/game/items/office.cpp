#include "office.h"

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


void OfficeItem::onPrepare()
{
	Item::onPrepare();
	backgroundSprite->texture = Texture::named("simtower/facilities/office/vacant");
	backgroundSprite->textureRect.size.x = 0.5;
}
