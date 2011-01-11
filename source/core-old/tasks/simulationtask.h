#ifndef OSS_SIMULATIONTASK_H
#define OSS_SIMULATIONTASK_H

#include "../../general.h"
#include "task.h"


namespace OSS {
	class SimulationTask : public Task {
	public:
		void update();
	};
}


#endif
