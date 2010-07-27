#ifndef SIMULATIONTASK_H
#define SIMULATIONTASK_H

#include "../../general.h"
#include "task.h"


namespace OSS {
	class SimulationTask : public Task {
	public:
		void update();
	};
}


#endif
