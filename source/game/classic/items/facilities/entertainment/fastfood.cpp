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

FastFoodItem::FastFoodItem(Tower * tower) : EntertainmentItem(tower, &descriptor)
{
	setVariant(randui(0, 4));
	setAutoTextured(true);
	setAutoTextureSlice(3);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Openness
//----------------------------------------------------------------------------------------------------

bool FastFoodItem::shouldBeOpen()
{
	return tower->time->isBetween(10, 21);
}

void FastFoodItem::didChangeOpenness()
{
	std::cout << " is " << (isOpen() ? "opening" : "closing") << std::endl;
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

void FastFoodItem::updateBackground()
{
	EntertainmentItem::updateBackground();	
	
	if (isOpen()) {
		setAutoTextureSlice(ceil((double)people.size() / 52 * 2));
	} else {
		setAutoTextureSlice(3);
	}
}

