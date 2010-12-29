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
#pragma mark Events
//----------------------------------------------------------------------------------------------------

bool OpenSkyscraper::handleEvent(CoreEvent * event)
{
	if (event->type == kCoreEventPrepare || event->type == kCoreEventCleanUp) {
		if (towerScene && towerScene->handleEvent(event)) return true;
	}
	return Application::handleEvent(event);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Birth and death
//----------------------------------------------------------------------------------------------------

void OpenSkyscraper::eventPrepare()
{	
	//Reload the resources of the SimTower singleton
	SimTower::shared()->loadResources();
	SimTower::shared()->extractAll();
	
	//Load an empty tower
	towerScene->tower = new Tower;
	
	//Switch to the tower scene
	Engine::shared()->switchToScene(towerScene);
}
