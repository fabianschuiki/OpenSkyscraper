#include "application.h"

#include "../classic/scenes/game.h"
#include "simtower.h"

using namespace OSS;
using namespace Game;


//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

Application::Application() : Engine::Application()
{
	//Initialize the SimTower singleton
	new SimTower(this);
	
	//Initialize the game scene with an empty tower
	gameScene = new Classic::GameScene(new Classic::Tower, engine);
}

Application::~Application()
{
	//Get rid of the scenes
	gameScene = NULL;
	
	//Get rid of SimTower
	delete SimTower::shared();
}

void Application::willRun()
{
	//Reload the resources of the SimTower singleton
	SimTower::shared()->loadResources();
	SimTower::shared()->extractAll();
	
	//Switch to the tower scene
	engine->setScene(gameScene);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Events
//----------------------------------------------------------------------------------------------------

/*bool Application::handleEvent(CoreEvent * event)
{
	if (event->type == kCoreEventPrepare || event->type == kCoreEventCleanUp) {
		if (towerScene && towerScene->handleEvent(event)) return true;
	}
	return Application::handleEvent(event);
}*/





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Birth and death
//----------------------------------------------------------------------------------------------------

/*void Application::eventPrepare()
{	
	//Reload the resources of the SimTower singleton
	SimTower::shared()->loadResources();
	SimTower::shared()->extractAll();
	
	//Load an empty tower
	towerScene->setTower(new Tower);
	
	//Switch to the tower scene
	Engine::shared()->switchToScene(towerScene);
}*/
