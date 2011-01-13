#ifndef OSS_GAME_APPLICATION_H
#define OSS_GAME_APPLICATION_H

#include "external.h"


namespace OSS {
	class SimTower;
	
	//Forward declare the classic game scene
	namespace Classic { class GameScene; }
	
	class OpenSkyscraper : public Application {
	public:
		//Scenes
		Pointer<Classic::GameScene> gameScene;
		
		//The OpenSkyscraper application maintains the SimTower singleton
		Pointer<SimTower> simtower;
		
		//Initialization
		OpenSkyscraper();
		
		void willRun();
	};
}


//Include the forward-declared stuff
#include "simtower.h"

#include "classic/game.h"


#endif
