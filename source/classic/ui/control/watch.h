#ifndef OSS_CLASSIC_UI_CONTROL_WATCH_H
#define OSS_CLASSIC_UI_CONTROL_WATCH_H

#include "../../external.h"


namespace OSS {
	namespace Classic {
		class ControlWindow;
		
		class WatchView : public GUI::View {
			
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
