#ifndef OSS_CLASSIC_UI_CONTROL_WINDOW_H
#define OSS_CLASSIC_UI_CONTROL_WINDOW_H

#include "../game.h"

#include "../../tower/tower.h"


namespace OSS {
	namespace Classic {
		class WatchView;
		class RatingView;
		
		class ControlWindow : public Window {
			
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
			Pointer<WatchView> watchView;
			Pointer<RatingView> ratingView;
			
			
			/**
			 * Drawing
			 */
		private:
			Pointer<Texture> backgroundTexture;
			
		public:
			virtual void draw(rectd dirtyRect);
		};
	}
}


#include "watch.h"
#include "rating.h"


#endif
