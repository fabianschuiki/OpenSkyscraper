#ifndef OSS_ENGINE_VIDEO_SPRITE_H
#define OSS_ENGINE_VIDEO_SPRITE_H

#include "../external.h"

#include "../resources/texture.h"


namespace OSS {
	namespace Engine {
		class Sprite : public Core::Object {
		public:
			//Initialization
			Sprite();
			~Sprite();
			
			/**
			 * Basic
			 */
			
			//TODO: Make rect and color private
			rectd rect;
			color4d color;
			bool hidden;
			
		public:
			const rectd & getRect();
			void setRect(const rectd & rect);
			
			const color4d & getColor();
			void setColor(const color4d & color);
			
			bool isHidden();
			void setHidden(bool hidden);
			
			
			//Texturing
			enum {
				kRepeatTextureMode,
				kRectTextureMode
			};
			Pointer<Texture> texture;
			rectd textureRect;
			unsigned int textureMode; //TODO: Make of type TextureMode
			double textureScale;
			
			//Autogenerating Texture Rect
			bool autoTexRectX;
			bool autoTexRectY;
			bool autoTexRelativeX;	//whether the texture coordinates should be relative to the sprite
			bool autoTexRelativeY;
			
			//Drawing
			void draw(rectd visibleRect);
			void autogenerateTextureRect();
		};
	}
}


#endif
