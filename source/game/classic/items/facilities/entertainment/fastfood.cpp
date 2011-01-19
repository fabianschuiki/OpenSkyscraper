#include "fastfood.h"

using namespace OSS;
using namespace Classic;


ItemDescriptor FastFoodItem::descriptor = {
	kFastFoodType,
	kEntertainmentGroup,
	kFacilityCategory,
	1,
	(0),
	100000,
	int2(16, 1)
};





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

FastFoodItem::FastFoodItem(Tower * tower) : FacilityItem(tower, &descriptor)
{
	setVariant(randui(0, 4));
	setAutoTextured(true);
	setAutoTextureSlice(0);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark State
//----------------------------------------------------------------------------------------------------

string FastFoodItem::getAutoTextureName(int floor, unsigned int slice)
{
	stringstream s;
	s << "simtower/facilities/fastfood/";
	s << getVariant();
	s << "/";
	s << (slice / 2);
	return s.str();
}

rectd FastFoodItem::getAutoTextureRect(int floor, unsigned int slice)
{
	return rectd((slice % 2) * 0.5, 0, 0.5, 1);
}
