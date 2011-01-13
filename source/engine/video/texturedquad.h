#ifndef OSS_ENGINE_VIDEO_TEXTUREDQUAD_H
#define OSS_ENGINE_VIDEO_TEXTUREDQUAD_H

#include "../external.h"

#include "../types.h"
#include "../base/pointer.h"
#include "../math/headers.h"
#include "../resources/texture.h"


namespace OSS {
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
		
		virtual void draw();
	};
}


#endif
