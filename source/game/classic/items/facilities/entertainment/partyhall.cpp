#include "partyhall.h"

using namespace OSS;
using namespace Classic;


ItemDescriptor PartyHallItem::descriptor = {
	kPartyHallType,
	kEntertainmentGroup,
	kFacilityCategory,
	1,
	(0),
	100000,
	int2(24, 2)
};





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

PartyHallItem::PartyHallItem(Tower * tower) : FacilityItem(tower, &descriptor)
{
	setAutoTextured(true);
	setAutoTextureSlice(2);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark State
//----------------------------------------------------------------------------------------------------

string PartyHallItem::getAutoTextureName(int floor, unsigned int slice)
{
	stringstream s;
	s << "simtower/facilities/partyhall/floor";
	s << floor;
	return s.str();
}

rectd PartyHallItem::getAutoTextureRect(int floor, unsigned int slice)
{
	return rectd(slice / 3.0, 0, 1.0 / 3, 1);
}
