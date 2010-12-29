#ifndef OSS_OPENGLCANVAS_H
#define OSS_OPENGLCANVAS_H

#include "../general.h"
#include "../base/base.h"
#include "coreobject.h"


namespace OSS {
	class OpenGLCanvas : public Singleton<OpenGLCanvas, CoreObject> {
	public:
		OpenGLCanvas();
		
		/**
		 * Video Mode
		 *
		 * There are three video mode variables. The desiredMode describes the
		 * optimal mode to be achieved and is the one the application is supposed
		 * to modify to express the user's preference. The safeMode describes
		 * the last mode the user confirmed to be working. The currentMode is
		 * rather self-explanatory and is NOT supposed to be modified by the
		 * application.
		 *
		 * Use activateMode() to try to switch to the app's desired video mode.
		 * If the user seems to be unable to confirm the mode, use
		 * revertToSafeMode() to restore the last mode known to work properly.
		 */
		struct VideoMode {
			int2 resolution;
			bool fullscreen;
		};
		
		//Video modes we need to keep track of
		VideoMode desiredMode; //the preferred mode
		VideoMode safeMode; //the last mode known to work
		VideoMode currentMode; //the mode the canvas currently is in
		
		//Applying video modes
		bool activateMode();
		void revertToSafeMode();
		void confirmCurrentModeToBeSafe();
	private:
		bool switchToMode(VideoMode * mode);
		
	public:
		//Buffer Swapping
		void swapBuffers();
		
		/**
		 * Events
		 */
		
		//Birth and death
		void eventPrepare();
		void eventCleanUp();
	};
}


#endif
