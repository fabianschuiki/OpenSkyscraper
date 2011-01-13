#ifndef OSS_ENGINE_SCENEOBJECT_H
#define OSS_ENGINE_SCENEOBJECT_H

#include "external.h"


namespace OSS {
	class SceneObject : public Object, public BasicResponder, public Responder, public Simulatable,
	public Updatable, public Drawable {
	public:
		SceneObject() : Responder(this) {}
	};
}


#endif
