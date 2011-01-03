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
	int2(9, 1)
};





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

OfficeItem::OfficeItem(Tower * tower) : FacilityItem(tower, &descriptor)
{
	type = randui(0, 6);
	vacant = true;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Type
//----------------------------------------------------------------------------------------------------

unsigned int OfficeItem::getType()
{
	return type;
}

void OfficeItem::setType(const unsigned int type)
{
	if (this->type != type) {
		this->type = type;
		if (!isVacant())
			updateBackground();
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Vacancy
//----------------------------------------------------------------------------------------------------

bool OfficeItem::isVacant()
{
	return vacant;
}

void OfficeItem::setVacant(const bool vacant)
{
	if (this->vacant != vacant) {
		this->vacant = vacant;
		updateBackground();
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Basic Sprites
//----------------------------------------------------------------------------------------------------

void OfficeItem::updateBackground()
{
	FacilityItem::updateBackground();
	
	//Load the appropriate office texture
	if (vacant) {
		backgrounds[0].texture = Texture::named("simtower/facilities/office/vacant");
		backgrounds[0].textureRect.size.x = 0.5;
		backgrounds[0].textureRect.origin.x = 0;
	} else {
		char str[256];
		sprintf(str, "simtower/facilities/office/inhabited/%i", type / 2);
		backgrounds[0].texture = Texture::named(str);
		backgrounds[0].textureRect.size.x = 0.25;
		backgrounds[0].textureRect.origin.x = (type % 2) * 0.5;
	}
	
	//Choose between day and night texture
	if (tower && (tower->time < 7 || tower->time >= 17))
		backgrounds[0].textureRect.origin.x += backgrounds[0].textureRect.size.x;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

void OfficeItem::advance(double dt)
{
	FacilityItem::advance(dt);
	
	static double route_t = 0;
	route_t += dt;
	if (route_t > 2) {
		route_t -= 2;
		Pointer<Route> route = tower->findRoute(tower->getGroundFloorRect(),
												getRect());
		if (route)
			OSSObjectLog << "found route " << route->description() << std::endl;
		else
			OSSObjectLog << "no route found" << std::endl;
	}
	
	//Update the background when the light state changes
	if (tower->checkTime(7) || tower->checkTime(17))
		updateBackground();
}
