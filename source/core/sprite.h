#ifndef OSS_SPRITE_H
#define OSS_SPRITE_H

#include "../general.h"
#include "../base/base.h"
#include "../resources/texture.h"
#include "coreobject.h"


namespace OSS {
	class Sprite : public CoreObject {
	public:
		//Texture modes
		enum {
			TextureModeRepeat,
			TextureModeRect
		};
		
		//Basic
		rectd rect;
		color4d color;
		
		//Texturing
		Pointer<Texture> texture;
		rectd textureRect;
		unsigned int textureMode;
		double textureScale;
		
		//Autogenerating Texture Rect
		bool autoTexRectX;
		bool autoTexRectY;
		
		//Initialization
		Sprite();
		~Sprite();
		
		//Drawing
		void draw(rectd visibleRect);
		void autogenerateTextureRect();
	};
}


#endif
