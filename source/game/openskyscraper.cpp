#include "openskyscraper.h"
#include "classes.h"

using namespace OSS;


//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

OpenSkyscraper::OpenSkyscraper() : Application()
{
	//Initialize the SimTower singleton
	new SimTower;
	
	//Initialize the scenes
	towerScene = new TowerScene;
}

OpenSkyscraper::~OpenSkyscraper()
{
	//Get rid of the scenes
	towerScene = NULL;
	
	//Get rid of SimTower
	delete SimTower::shared();
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Birth and death
//----------------------------------------------------------------------------------------------------

void OpenSkyscraper::onPrepare()
{
	//Reload the resources of the SimTower singleton
	SimTower::shared()->reloadResources();
	
	//Create an empty tower
	towerScene->tower = new Tower;
	
	//Switch to the tower scene
	Engine::shared()->switchToScene(towerScene);
}

void OpenSkyscraper::onCleanUp()
{
}
