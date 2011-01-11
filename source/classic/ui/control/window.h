#ifndef OSS_CLASSIC_UI_CONTROL_WINDOW_H
#define OSS_CLASSIC_UI_CONTROL_WINDOW_H

#include "../../external.h"


namespace OSS {
	namespace Classic {
		class ControlWindow : public GUI::View {
			
			/**
			 * Construction
			 */
		public:
			ControlWindow();
			
			
			/**
			 * Drawing
			 */
		private:
			Pointer<Engine::Texture> backgroundTexture;
			
		public:
			virtual void draw(rectd dirtyRect);
		};
	}
}


#endif
