#include "application.h"

using namespace OSS;


//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

OpenSkyscraper::OpenSkyscraper()
{
	//Initialize the SimTower singleton
	simtower = new SimTower(this);
	
	//Initialize the game scene with an empty tower
	gameScene = new Classic::GameScene(new Classic::Tower, engine);
}

void OpenSkyscraper::willRun()
{
	//Reload the resources of the SimTower singleton
	simtower->loadResources();
	simtower->extractAll();
	
	//Switch to the tower scene
	engine->setScene(gameScene);
}
