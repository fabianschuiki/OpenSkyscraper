#ifndef OSS_TASK_H
#define OSS_TASK_H

#include "../../general.h"
#include "../coreobject.h"


namespace OSS {
	class Task : public CoreObject {
	public:
		virtual void update() {}
	};
}


#endif
