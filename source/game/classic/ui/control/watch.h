#ifndef OSS_GAME_CLASSIC_UI_CONTROL_WATCH_H
#define OSS_GAME_CLASSIC_UI_CONTROL_WATCH_H

#include "window.h"


namespace OSS {
	namespace Classic {		
		class WatchView : public View {
			
			/**
			 * Construction
			 */
		public:
			const Pointer<ControlWindow> controlWindow;
			
			WatchView(ControlWindow * window);
			
			
			/**
			 * Drawing
			 */
		public:
			virtual void draw(rectd dirtyRect);
		};
	}
}


#endif
