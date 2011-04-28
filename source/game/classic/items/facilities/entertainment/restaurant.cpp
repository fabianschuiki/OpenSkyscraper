#include "restaurant.h"

using namespace OSS;
using namespace Classic;


ItemDescriptor RestaurantItem::descriptor = {
	kRestaurantType,
	kEntertainmentGroup,
	kFacilityCategory,
	1,
	(0),
	200000,
	int2(24, 1)
};





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

RestaurantItem::RestaurantItem(Tower * tower) : EntertainmentItem(tower, &descriptor)
{
	setVariant(randui(0, 4));
	setAutoTextured(true);
	setAutoTextureSlice(0);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Openness
//----------------------------------------------------------------------------------------------------

bool RestaurantItem::shouldBeOpen()
{
	return tower->time->isBetween(17, 23);
}

void RestaurantItem::didChangeOpenness()
{
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark State
//----------------------------------------------------------------------------------------------------

string RestaurantItem::getAutoTextureName(int floor, unsigned int slice)
{
	stringstream s;
	s << "simtower/facilities/restaurant/";
	s << getVariant();
	s << "/";
	s << (slice / 2);
	return s.str();
}

rectd RestaurantItem::getAutoTextureRect(int floor, unsigned int slice)
{
	return rectd((slice % 2) * 0.5, 0, 0.5, 1);
}

void RestaurantItem::updateBackground()
{
	EntertainmentItem::updateBackground();
	
	if (isOpen()) {
		setAutoTextureSlice(ceil((double)people.size() / 52 * 2)); //TODO: lookup max number of guests
	} else {
		setAutoTextureSlice(3);
	}
}

