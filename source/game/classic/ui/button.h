#ifndef OSS_CLASSIC_UI_BUTTON_H
#define OSS_CLASSIC_UI_BUTTON_H

#include "../../ui/view.h"


namespace OSS {
	namespace Classic {
		class Button : public View {
		public:
			Button();
			
			bool pressed;
			bool disabled;
			
			Pointer<Texture> normalTexture;
			Pointer<Texture> pressedTexture;
			Pointer<Texture> disabledTexture;
			rectd normalTextureRect;
			rectd pressedTextureRect;
			rectd disabledTextureRect;
			
			virtual void draw(rectd dirtyRect);
			
			
			/**
			 * Events
			 */
		public:
			virtual bool eventMouseDown(MouseButtonEvent * event);
			
			
			/**
			 * Actions
			 */
		public:
			sigc::signal<void> onClicked;
		};
	}
}


#endif
