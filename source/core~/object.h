#ifndef OSS_CORE_OBJECT_H
#define OSS_CORE_OBJECT_H

#include "../base/object~.h"
#include "objectextension.h"


namespace OSS {
	namespace Core {
		class Object : public Base::Object, public ObjectExtension {
		};
	}
}


#endif
