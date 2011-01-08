#ifndef OSS_GAME_OPENSKYSCRAPER_H
#define OSS_GAME_OPENSKYSCRAPER_H

#include "external.h"

#include "../classic/scenes/game.h"


namespace OSS {
	namespace Game {
		class OpenSkyscraper : public Engine::Application {
		public:
			//Scenes
			Pointer<Classic::GameScene> gameScene;
			
			//Initialization
			OpenSkyscraper();
			~OpenSkyscraper();
			
			void willRun();
		};
	}
}


#endif
