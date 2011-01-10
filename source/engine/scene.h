#ifndef OSS_ENGINE_SCENE_H
#define OSS_ENGINE_SCENE_H

#include "responder.h"

#include "engine.h"


namespace OSS {
	namespace Engine {
		class Scene : public Responder {
			
			/**
			 * Construction
			 */
		public:
			const Base::Pointer<EngineCore> engine;
			
			Scene(EngineCore * engine);
			
			
			/**
			 * Presentation
			 */
		public:
			bool isOnScreen();
			
			virtual void willMoveOnScreen() {}
			virtual void didMoveOnScreen();
			
			virtual void willMoveOffScreen() {}
			virtual void didMoveOffScreen() {}
			
			
			/**
			 * Visual Convenience
			 *
			 * The following to functions provide an easy way to update the view-
			 * port and projection matrix of the scene. Both, updateViewport() and updateProjection()
			 * are called automatically by the default implementation of the videoModeChanged event of
			 * the scene. This means that in most cases you don't have to implement the event yourself,
			 * but can rely on the higher level functions updateViewport() and updateProjection() to
			 * adjust your visuals.
			 *
			 * The default implementation of updateViewport() simply sets the viewport to cover the
			 * entire OpenGLCanvas, as this probably is the most common setting.
			 *
			 * The default implementation of updateProjection() creates an orthogonal projection matrix
			 * so that the coordinates in the modelview matrix map directly to the pixels of the canvas.
			 */
		public:
			virtual void updateViewport();
			virtual void updateProjection();
			
			
			/**
			 * Event Handling
			 */
		public:
			virtual void eventVideoChanged(VideoEvent * event);
		};
	}
}


#endif
