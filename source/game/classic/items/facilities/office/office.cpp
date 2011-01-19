#include "office.h"

using namespace OSS;
using namespace Classic;


ItemDescriptor OfficeItem::descriptor = {
	kOfficeType,
	kOfficeGroup,
	kFacilityCategory,
	1,
	(kNotBelowGroundAttribute),
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
		
		//Transfer some funds
		tower->funds->transfer(vacant ? -10000 : 10000);
		
		//Populate or clear the office
		if (vacant)
			clearWorkers();
		else
			initWorkers();
		
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
		stringstream str;
		str << "simtower/facilities/office/inhabited/" << (type / 2);
		backgrounds[0].texture = Texture::named(str.str());
		backgrounds[0].textureRect.size.x = 0.25;
		backgrounds[0].textureRect.origin.x = (type % 2) * 0.5;
	}
	
	//Choose between day and night texture
	if (tower && (tower->time->getTime() < 7 || tower->time->getTime() >= 17))
		backgrounds[0].textureRect.origin.x += backgrounds[0].textureRect.size.x;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

void OfficeItem::advance(double dt)
{
	FacilityItem::advance(dt);
	if (underConstruction) return;
	
	//Handle vacant attractive offices
	if (isVacant() && isAttractiveForUse()) {
		//Reset the occupancy time at 5 in the morning
		if (tower->checkTime(5))
			occupancyTime = 0;
		
		//Check whether it is time for occupancy operations
		if (tower->time->getTime() >= 7 && tower->time->getTime() < 17) {
			//If the occupancy time is invalid, set it to a proper value
			if (occupancyTime < 7 || occupancyTime >= 17) {
				occupancyTime = randd(tower->time->getTime(), std::min<double>(tower->time->getTime() + 2, 17));
				OSSObjectLog << "reset occupancy time to " << occupancyTime << std::endl;
			}
			
			//If we just hit the occupancy time, set the vacancy to false
			if (tower->checkTime(occupancyTime))
				setVacant(false);
		}
	}
	
	//Move out of unattractive offices monday morning
	if (!isVacant() && !isAttractiveForUse() && tower->time->getDayOfWeek() == 0 && tower->checkTime(5))
		setVacant(true);
	
	//Update worker schedules
	/*if (!isVacant() && tower->checkTime(5))
		updateWorkerSchedules();*/
	if (!isVacant())
		advanceWorkers(dt);
	
	//Update the worker schedules
	for (WorkerMap::iterator w = workers.begin(); w != workers.end(); w++)
		if (w->second)
			w->second->updateTimedDestination();
	
	//DEBUG: Colorize the office if it is not reachable from the lobby
	backgrounds[0].color = (isReachableFromLobby() ? (color4d){1, 1, 1, 1} : (color4d){1, 0.25, 0.25, 1});
	
	//Update the background when the light state changes
	if (tower->checkTime(7) || tower->checkTime(17))
		updateBackground();
}

void OfficeItem::advanceWorkers(double dt)
{
	bool resetWorkers = tower->checkTime(3);
	
	//Iterate through the workers and advance those that haven't got a clue what to do :)
	for (WorkerMap::iterator w = workers.begin(); w != workers.end(); w++) {
		if (resetWorkers)
			w->second->reset();
		if (w->second->hasNoPlans())
			advanceWorker(w->first, w->second);
	}
}

void OfficeItem::advanceWorker(string key, TimedPerson * worker)
{
	OSSObjectLog << "advancing worker " << key << ": ";
	
	//Before 12:00, move to the office
	if (tower->time->getTime() < 12) {
		if (worker->isAt(this)) {
			std::cout << "work at office until 12:00";
			worker->setPauseEndTime(12);
		} else {
			worker->setNextDestination(randd(std::max<double>(tower->time->getTime(), 7), 10), this, 0.25);
			std::cout << "go to work at " << worker->getNextDestinationTime();
		}
	}
	
	//Between 12:00 and 12:45, go have lunch if at the office
	else if (tower->time->getTime() >= 12 && tower->time->getTime() < 12.75 && worker->isAt(this) &&
			 !worker->boolProps["hadLunch"]) {
		worker->setNextDestination(randd(tower->time->getTime(), tower->time->getTime() + 1/6.0), NULL, 0.25);
		std::cout << "go to have lunch at " << worker->getNextDestinationTime();
	}
	
	//Between 12:00 and 17:00, return from lunch after spending 20 minutes there
	else if (tower->time->getTime() >= 12 && tower->time->getTime() < 17) {
		if (worker->isAt(this)) {
			std::cout << "work at office until 17:00";
			worker->setPauseEndTime(17);
		} else {
			worker->setNextDestination(randd(tower->time->getTime(), tower->time->getTime() + 10 / 60.0), this, 0.25);
			std::cout << "go back to work at " << worker->getNextDestinationTime();
			worker->boolProps["hadLunch"] = true;
		}
	}
	
	//After 17:00, go home
	else if (tower->time->getTime() >= 17) {
		if (worker->isAt(NULL)) {
			std::cout << "enjoy evening at home";
			worker->setPauseEndTime(24);
		} else {
			worker->setNextDestination(randd(tower->time->getTime(), 19), NULL);
			std::cout << "go home at " << worker->getNextDestinationTime();
		}
	}
	
	std::cout << std::endl;
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
	/*if (!route)
		route = Route::findRoute(tower, tower->getGroundFloorRect(), getRect());*/
	
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





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Workers
//----------------------------------------------------------------------------------------------------

void OfficeItem::initWorkers()
{
	//Initialize the salesmen
	/*workers["salesman/0"] = new TimedPerson(tower);
	workers["salesman/1"] = new TimedPerson(tower);
	
	//Initialize the male workers
	workers["male/0"] = new TimedPerson(tower);
	workers["male/1"] = new TimedPerson(tower);
	
	//Initialize the female workers
	workers["female/0"] = new TimedPerson(tower);*/
	workers["female/1"] = new TimedPerson(tower);
	
	//updateWorkerSchedules();
}

void OfficeItem::clearWorkers()
{
	workers.clear();
}

/*void OfficeItem::updateWorkerSchedules()
{
	OSSObjectLog << std::endl;
	
	//Update the salesmen
	updateSalesmanSchedule(workers["salesman/0"]);
	updateSalesmanSchedule(workers["salesman/1"]);
	
	//Update the other workers
	updateWorkerSchedule(workers["male/0"]);
	updateWorkerSchedule(workers["male/1"]);
	updateWorkerSchedule(workers["female/0"]);
	updateWorkerSchedule(workers["female/1"]);
}

void OfficeItem::updateSalesmanSchedule(ScheduledPerson * person)
{
	if (!person)
		return;
	
	Schedule * s = new Schedule;
	
	//Arrive at the tower
	s->addNode(randd(7, 10), this);
	
	//Leave the tower
	s->addNode(randd(17, 18.5), NULL);
	person->setSchedule(s);
}

void OfficeItem::updateWorkerSchedule(ScheduledPerson * person)
{
	if (!person)
		return;
	
	Schedule * s = new Schedule;
}*/
