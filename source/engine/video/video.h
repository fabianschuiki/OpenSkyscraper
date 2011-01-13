#ifndef OSS_ENGINE_VIDEO_VIDEO_H
#define OSS_ENGINE_VIDEO_VIDEO_H

#include "../external.h"
#include "../base/object.h"

#include "../base/pointer.h"
#include "../engine.h"


namespace OSS {	
	class Video : public Object, public Singleton<Video> {
		
		/**
		 * Initialization
		 */
	public:
		const Pointer<Engine> engine;
		
		Video(Engine * engine);
		~Video();
		
		
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
	public:
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
		
		//Buffer Swapping
	public:
		void swapBuffers();
	};
}


#endif
