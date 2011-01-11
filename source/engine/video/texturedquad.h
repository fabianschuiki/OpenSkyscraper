#ifndef OSS_ENGINE_VIDEO_TEXTUREDQUAD_H
#define OSS_ENGINE_VIDEO_TEXTUREDQUAD_H

#include "../external.h"

#include "../resources/texture.h"


namespace OSS {
	namespace Engine {
		class TexturedQuad {
		public:
			TexturedQuad();
			
			rectd rect;
			
			color4d color;
			double alpha;
			
			Pointer<Texture> texture;
			rectd textureRect;
			void autogenerateTextureRect(bool x, bool y, double2 offset = double2(),
										 double2 scale = double2(1, 1));
			
			void draw();
		};
	}
}


#endif
