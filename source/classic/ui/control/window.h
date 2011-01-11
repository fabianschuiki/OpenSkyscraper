#ifndef OSS_CLASSIC_UI_CONTROL_WINDOW_H
#define OSS_CLASSIC_UI_CONTROL_WINDOW_H

#include "../../external.h"

#include "../../tower/tower.h"


namespace OSS {
	namespace Classic {
		class GameUI;
		
		class ControlWindow : public GUI::View {
			
			/**
			 * Construction
			 */
		public:
			const Pointer<GameUI> ui;
			
			ControlWindow(GameUI * ui);
			
			Tower * getTower();
			
			
			/**
			 * Subviews
			 */
			
			
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
