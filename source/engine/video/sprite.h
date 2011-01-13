#ifndef OSS_ENGINE_VIDEO_SPRITE_H
#define OSS_ENGINE_VIDEO_SPRITE_H

#include "../external.h"
#include "../base/object.h"
#include "texturedquad.h"


namespace OSS {
	class Sprite : public Object, public TexturedQuad {
	public:
		//Initialization
		Sprite();
		
		//Attributes
		bool hidden;
		
		//Automated texture reect calculation
		enum {
			kAutoX			= (1 << 0),
			kAutoY			= (1 << 1),
			kRelative		= (1 << 2)
		};			
		unsigned int autoTexRectAttributes;
		double2 autoTexRectScale;
		rectd::Point autoTexRectRelativeTo;
		
		//Drawing
		virtual void draw();
	};
}


#endif
