#include "openskyscraper.h"
#include "classes.h"

using namespace OSS;


//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

OpenSkyscraper::OpenSkyscraper() : Application()
{
	//Initialize the scenes
	towerScene = new TowerScene;
}

OpenSkyscraper::~OpenSkyscraper()
{
	//Get rid of the scenes
	towerScene = NULL;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Birth and death
//----------------------------------------------------------------------------------------------------

void OpenSkyscraper::onPrepare()
{
	//Create an empty tower
	towerScene->tower = new Tower;
	
	//Switch to the tower scene
	Engine::shared()->switchToScene(towerScene);
}

void OpenSkyscraper::onCleanUp()
{
}
