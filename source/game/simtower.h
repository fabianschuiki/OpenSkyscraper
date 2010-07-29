#ifndef OSS_SIMTOWER_H
#define OSS_SIMTOWER_H

#include "../general.h"
#include "../base/base.h"


namespace OSS {
	class SimTower : public Singleton<SimTower, Object> {
	public:
		void reloadResources();
	};
}


#endif
