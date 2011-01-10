#ifndef OSS_ENGINE_OBJECT_H
#define OSS_ENGINE_OBJECT_H

#include "external.h"

#include "drawable.h"
#include "simulatable.h"


namespace OSS {
	namespace Engine {
		class Object : public Core::Object, public Drawable, public Simulatable {
		};
	}
}


#endif
