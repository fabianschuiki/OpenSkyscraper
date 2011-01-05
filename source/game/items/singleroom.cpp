#include "singleroom.h"
#include "../tower.h"

using namespace OSS;


Item::Descriptor SingleRoomItem::descriptor = {
	Item::kSingleRoomType,
	Item::kHotelGroup,
	Item::kFacilityCategory,
	1,
	(Item::kNotBelowGroundAttribute),
	20000,
	int2(4, 1)
};





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

SingleRoomItem::SingleRoomItem(Tower * tower) : FacilityItem(tower, &descriptor)
{
	type = randui(0, 1);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Type
//----------------------------------------------------------------------------------------------------

unsigned int SingleRoomItem::getType()
{
	return type;
}

void SingleRoomItem::setType(const unsigned int type)
{
	if (this->type != type) {
		this->type = type;
		updateBackground();
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Basic Sprites
//----------------------------------------------------------------------------------------------------

void SingleRoomItem::updateBackground()
{
	FacilityItem::updateBackground();
	
	//Load the appropriate hotel texture
	char str[256];
	sprintf(str, "simtower/facilities/hotel/single/%i/1", type);
	backgrounds[0].texture = Texture::named(str);
	backgrounds[0].textureRect = rectd(0.125 * 2, 0, 0.125, 1);
}
