#ifndef OSS_GAME_GAMEOBJECT_H
#define OSS_GAME_GAMEOBJECT_H

#include "external.h"


namespace OSS {
	namespace Classic {
		class GameObject : public SceneObject, public Classic::Responder {
		public:
			GameObject() : Classic::Responder(this) {}
			
			//The type name should be some string identifying this object. It may be used for
			//example in implementations of Item where you have to distinguish between different
			//kinds of people moving to and from your item.
			virtual string getTypeName() { return ""; }
		};
	}
}


#endif
