#include "shop.h"

using namespace OSS;
using namespace Classic;


ItemDescriptor ShopItem::descriptor = {
	kShopType,
	kEntertainmentGroup,
	kFacilityCategory,
	1,
	(0),
	100000,
	int2(12, 1)
};





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

ShopItem::ShopItem(Tower * tower) : FacilityItem(tower, &descriptor)
{
	setVariant(randui(0, 10));
	setAutoTextured(true);
	setAutoTextureSlice(0);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark State
//----------------------------------------------------------------------------------------------------

string ShopItem::getAutoTextureName(int floor, unsigned int slice)
{
	stringstream s;
	s << "simtower/facilities/shop/";
	s << getVariant();
	return s.str();
}

rectd ShopItem::getAutoTextureRect(int floor, unsigned int slice)
{
	return rectd(slice / 3.0, 0, 1.0 / 3, 1);
}
