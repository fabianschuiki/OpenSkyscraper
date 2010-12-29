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
			kRepeatTextureMode,
			kRectTextureMode
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
		bool autoTexRelativeX;	//whether the texture coordinates should be relative to the sprite
		bool autoTexRelativeY;
		
		//Initialization
		Sprite();
		~Sprite();
		
		//Drawing
		void draw(rectd visibleRect);
		void autogenerateTextureRect();
	};
}


#endif
