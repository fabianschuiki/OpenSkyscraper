#ifndef OSS_LOADERTASK_H
#define OSS_LOADERTASK_H

#include "../../general.h"
#include "task.h"
#include "../../resources/store.h"


namespace OSS {
	class LoaderTask : public Task {
	public:
		void update();
	};
}


#endif
