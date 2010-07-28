#include "loadertask.h"
#include "../core.h"

#define BREAKOUT_TIME	0.010

using namespace OSS;


void LoaderTask::update()
{
	double start = Platform::shared()->getTimeElapsed();
	double now = start;
	Engine::StoreList * stores = &Engine::shared()->stores;
	for (Engine::StoreList::iterator store = stores->begin(); store != stores->end() && (now - start < BREAKOUT_TIME); store++) {
		bool workDone = true;
		while (workDone && (now - start < BREAKOUT_TIME)) {
			now = Platform::shared()->getTimeElapsed();
			if ((*store)->loadNext()) continue;
			if ((*store)->finalizeNext()) continue;
			if ((*store)->unfinalizeNext()) continue;
			if ((*store)->unloadNext()) continue;
			workDone = false;
		}
	}
}
