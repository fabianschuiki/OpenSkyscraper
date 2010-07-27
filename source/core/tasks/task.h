#ifndef TASK_H
#define TASK_H

#include "../../general.h"
#include "../coreobject.h"


namespace OSS {
	class Task : public CoreObject {
	public:
		virtual void update() {}
	};
}


#endif
