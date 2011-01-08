#ifndef OSS_ENGINE_DRAWABLE_H
#define OSS_ENGINE_DRAWABLE_H

#include "external.h"

#include "scene.h"


namespace OSS {
	namespace Engine {
		class Drawable {
			
			/**
			 * Drawing
			 */
		public:
			virtual void draw(rectd dirtyRect) {}
			virtual void draw(Scene * scene, rectd dirtyRect) { draw(dirtyRect); }
		};
	}
}


#endif
