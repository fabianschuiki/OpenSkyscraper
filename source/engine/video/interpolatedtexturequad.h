#ifndef OSS_ENGINE_VIDEO_INTERPOLATEDTEXTUREQUAD_H
#define OSS_ENGINE_VIDEO_INTERPOLATEDTEXTUREQUAD_H

#include "../external.h"

#include "texturedquad.h"


namespace OSS {
	namespace Engine {
		class InterpolatedTextureQuad {
		public:
			InterpolatedTextureQuad();
			
			rectd rect;
			
			TexturedQuad state0;
			TexturedQuad state1;
			void autogenerateTextureRect(bool x, bool y, double2 offset = double2(),
										 double2 scale = double2(1, 1));
			
			double interpolation;
			
			void draw();
		};
	}
}


#endif
