#ifndef OSS_OPENSKYSCRAPER_H
#define OSS_OPENSKYSCRAPER_H

#include "../general.h"
#include "../core/core.h"
#include "scenes/scenes.h"


namespace OSS {
	class OpenSkyscraper : public Application {
	public:
		//Scenes
		Pointer<TowerScene> towerScene;
		
		//Initialization
		OpenSkyscraper();
		~OpenSkyscraper();
		
		//Events
		virtual bool handleEvent(CoreEvent * event);
		
		//Birth and death
		virtual void eventPrepare();
	};
}


#endif
