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
	occupancyTime = 0;
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
		tower->transferFunds(vacant ? -10000 : 10000);
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
	
	//Handle vacant attractive offices
	if (isVacant() && isAttractiveForUse()) {
		//Reset the occupancy time at 5 in the morning
		if (tower->checkTime(5))
			occupancyTime = 0;
		
		//If the occupancy time is invalid, set it to a proper value
		if (occupancyTime < 7 || occupancyTime >= 17)
			occupancyTime = randd(tower->time, std::min<double>(tower->time + 1, 17));
		
		//If we just hit the occupancy time, set the vacancy to false
		if (tower->checkTime(occupancyTime))
			setVacant(false);
	}
	
	//Move out of unattractive offices monday morning
	if (!isVacant() && !isAttractiveForUse() && tower->getDayOfWeek() == 0 && tower->checkTime(5))
		setVacant(true);
	
	//DEBUG: Colorize the office if it is not reachable from the lobby
	backgrounds[0].color = (isReachableFromLobby() ? (color4d){1, 1, 1, 1} : (color4d){1, 0.25, 0.25, 1});
	
	//Update the background when the light state changes
	if (tower->checkTime(7) || tower->checkTime(17))
		updateBackground();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Reachability
//----------------------------------------------------------------------------------------------------

Route * OfficeItem::getRouteFromLobby() const
{
	return routeFromLobby;
}

void OfficeItem::setRouteFromLobby(Route * route)
{
	if (routeFromLobby != route) {
		routeFromLobby = route;
	}
}

bool OfficeItem::isReachableFromLobby()
{
	return (getRouteFromLobby() != NULL);
}

void OfficeItem::updateRouteFromLobby()
{
	//If the route from the lobby exists but is invalid, get rid of it
	Route * route = getRouteFromLobby();
	if (route && !route->isValid()) {
		OSSObjectLog << "route is invalid, setting to NULL" << std::endl;
		route = NULL;
	}
	
	//If there's no route from the lobby, try to calculate one
	if (!route)
		route = tower->findRoute(tower->getGroundFloorRect(), getRect());
	
	//Store the route
	setRouteFromLobby(route);
	
	//DEBUG: Log the route
	if (routeFromLobby)
		OSSObjectLog << routeFromLobby->description() << std::endl;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Attractivity
//----------------------------------------------------------------------------------------------------

bool OfficeItem::isAttractiveForUse()
{
	return isReachableFromLobby();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Notifications
//----------------------------------------------------------------------------------------------------

void OfficeItem::onChangeLocation()
{
	FacilityItem::onChangeLocation();
	updateRouteFromLobby();
}

void OfficeItem::onChangeTransportItems()
{
	FacilityItem::onChangeTransportItems();
	updateRouteFromLobby();
}
