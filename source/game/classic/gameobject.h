#ifndef OSS_GAME_GAMEOBJECT_H
#define OSS_GAME_GAMEOBJECT_H

#include "external.h"


namespace OSS {
	namespace Classic {
		class GameObject : public SceneObject, public Classic::Responder {
		public:
			GameObject() : Classic::Responder(this) {}
		};
	}
}


#endif
