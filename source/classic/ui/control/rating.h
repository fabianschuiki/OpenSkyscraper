#ifndef OSS_CLASSIC_UI_CONTROL_RATING_H
#define OSS_CLASSIC_UI_CONTROL_RATING_H

#include "../../external.h"


namespace OSS {
	namespace Classic {
		class ControlWindow;
		
		class RatingView : public GUI::View {
			
			/**
			 * Construction
			 */
		public:
			const Pointer<ControlWindow> controlWindow;
			
			RatingView(ControlWindow * window);
			
			
			/**
			 * Drawing
			 */
		private:
			Pointer<Engine::Texture> emptyStarTexture;
			Pointer<Engine::Texture> filledStarTexture;
			
			Pointer<Engine::Texture> towerRatingTexture;
			
		public:
			virtual void draw(rectd dirtyRect);
		};
	}
}


#endif
