#ifndef OSS_ENGINE_DRAWABLE_H
#define OSS_ENGINE_DRAWABLE_H

#include "../math/rect.h"


namespace OSS {
	namespace Engine {
		class Drawable {
			
			/**
			 * Drawing
			 */
		public:
			virtual void draw(rectd dirtyRect) {}
		};
	}
}


#endif
