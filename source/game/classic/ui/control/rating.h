#ifndef OSS_CLASSIC_UI_CONTROL_RATING_H
#define OSS_CLASSIC_UI_CONTROL_RATING_H

#include "window.h"


namespace OSS {
	namespace Classic {
		class RatingView : public View {
			
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
			Pointer<Texture> emptyStarTexture;
			Pointer<Texture> filledStarTexture;
			
			Pointer<Texture> towerRatingTexture;
			
		public:
			virtual void draw(rectd dirtyRect);
		};
	}
}


#endif
