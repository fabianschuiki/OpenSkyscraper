#include "environment.h"

#include "tower.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

TowerEnvironment::TowerEnvironment(Tower * tower) : tower(tower),
updatePopulationIfNeeded(this, &TowerEnvironment::updatePopulation)
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
#pragma mark State
//----------------------------------------------------------------------------------------------------

void TowerEnvironment::update()
{
	updatePopulationIfNeeded();
}

void TowerEnvironment::updatePopulation()
{
	//TODO: iterate through all the items and update the population
}
