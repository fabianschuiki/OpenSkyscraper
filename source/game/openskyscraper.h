#ifndef OSS_OPENSKYSCRAPER_H
#define OSS_OPENSKYSCRAPER_H

#include "../engine/application.h"

#include "scenes/towerscene.h"


namespace OSS {
	class OpenSkyscraper : public Engine::Application {
	public:
		//Scenes
		Pointer<TowerScene> towerScene;
		
		//Initialization
		OpenSkyscraper();
		~OpenSkyscraper();
		
		void willRun();
	};
}


#endif
