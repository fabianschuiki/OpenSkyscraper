#ifndef OSS_ENGINE_SCENE_H
#define OSS_ENGINE_SCENE_H

#include "responder.h"
#include "drawable.h"
#include "simulatable.h"

#include "engine.h"


namespace OSS {
	namespace Engine {
		class Scene : public Responder, public Drawable, public Simulatable {
			
			/**
			 * Construction
			 */
		public:
			const Base::Pointer<EngineCore> engine;
			
			Scene(EngineCore * engine);
			
			
			/**
			 * Presentation
			 */
		public:
			bool isOnScreen();
			
			virtual void willMoveOnScreen() {}
			virtual void didMoveOnScreen() {}
			
			virtual void willMoveOffScreen() {}
			virtual void didMoveOffScreen() {}
		};
	}
}


#endif
