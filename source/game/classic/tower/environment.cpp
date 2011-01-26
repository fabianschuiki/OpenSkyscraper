#include "environment.h"

#include "../people/person.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

TowerEnvironment::TowerEnvironment(Tower * tower) : tower(tower),
updatePopulationIfNeeded(this, &TowerEnvironment::updatePopulation, &updateIfNeeded),
updatePeopleIfNeeded(this, &TowerEnvironment::updatePeople, &updateIfNeeded)
{
	rating = 1;
	population = 0;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Rating
//----------------------------------------------------------------------------------------------------

unsigned short TowerEnvironment::getRating()
{
	return rating;
}

void TowerEnvironment::setRating(unsigned short r)
{
	if (rating != r) {
		rating = r;
		//TODO: Play the ominous sound somehow...
	}
}

unsigned int TowerEnvironment::getLobbyStyle()
{
	if (rating >= 4)
		return 2;
	if (rating >= 3)
		return 1;
	return 0;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Population
//----------------------------------------------------------------------------------------------------

unsigned int TowerEnvironment::getPopulation()
{
	return population;
}

void TowerEnvironment::setPopulation(unsigned int p)
{
	if (population != p) {
		population = p;
	}
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark People
//----------------------------------------------------------------------------------------------------

void TowerEnvironment::addPerson(Person * p)
{
	if (!p) return;
	//OSSObjectLog << "created person " << p->description() << std::endl;
	existingPeople.insert(p);
	
	//Make sure the update needs are propagated appropriately to this instance
	p->updateIfNeeded.parent = &updatePeopleIfNeeded;
}

void TowerEnvironment::removePerson(Person * p)
{
	if (!p) return;
	//OSSObjectLog << "killed person " << p->description() << std::endl;
	existingPeople.erase(p);
	
	//Undo the update propagation
	p->updateIfNeeded.parent = NULL;
}

void TowerEnvironment::addPersonToTower(Person * p)
{
	if (!p) return;
	//OSSObjectLog << p->description() << " moved in" << std::endl;
	peopleInTower.insert(p);
}

void TowerEnvironment::removePersonFromTower(Person * p)
{
	if (!p) return;
	OSSObjectLog << p->description() << " moved out" << std::endl;
	peopleInTower.erase(p);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Simulation
//----------------------------------------------------------------------------------------------------

void TowerEnvironment::advance(double dt)
{
	advancePeople(dt);
}

void TowerEnvironment::advancePeople(double dt)
{
	//Iterate through the existing people and ask each to advance.
	for (set<Person *>::iterator it = existingPeople.begin(); it != existingPeople.end(); it++)
		(*it)->advance(dt);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark State
//----------------------------------------------------------------------------------------------------

void TowerEnvironment::update()
{
	updatePopulationIfNeeded();
	
	//Update the people
	updatePeopleIfNeeded();
}

void TowerEnvironment::updatePopulation()
{
	//TODO: iterate through all the items and update the population
}

void TowerEnvironment::updatePeople()
{
	//Iterate through the existing people and ask each to update itself.
	for (set<Person *>::iterator it = existingPeople.begin(); it != existingPeople.end(); it++)
		(*it)->updateIfNeeded();
}
