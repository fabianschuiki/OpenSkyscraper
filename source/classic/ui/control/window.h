#ifndef OSS_CLASSIC_UI_CONTROL_WINDOW_H
#define OSS_CLASSIC_UI_CONTROL_WINDOW_H

#include "../../external.h"

#include "../../tower/tower.h"
#include "watch.h"


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
			
			//TODO: move this somewhere else
			Tower * getTower();
			
			
			/**
			 * Subviews
			 */
		private:
			Pointer<Watch> watchView;
			
			
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
