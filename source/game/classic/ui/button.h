#ifndef OSS_CLASSIC_UI_BUTTON_H
#define OSS_CLASSIC_UI_BUTTON_H

#include "../../ui/view.h"


namespace OSS {
	namespace Classic {
		class Button : public View {
		public:
			Button();
			
			bool pressed;
			
			Pointer<Texture> normalTexture;
			Pointer<Texture> pressedTexture;
			rectd normalTextureRect;
			rectd pressedTextureRect;
			
			virtual void draw(rectd dirtyRect);
		};
	}
}


#endif
