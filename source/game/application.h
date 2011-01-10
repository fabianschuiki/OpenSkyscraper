#ifndef OSS_GAME_APPLICATION_H
#define OSS_GAME_APPLICATION_H

#include "external.h"


namespace OSS {
	namespace Classic { class GameScene; }
	
	namespace Game {		
		class Application : public Engine::Application {
		public:
			//Scenes
			Pointer<Classic::GameScene> gameScene;
			
			//Initialization
			Application();
			~Application();
			
			void willRun();
		};
	}
}


#endif
