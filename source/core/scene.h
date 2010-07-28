#ifndef OSS_SCENE_H
#define OSS_SCENE_H

#include "../general.h"
#include "coreobject.h"


namespace OSS {
	class Scene : public CoreObject {
	public:
		/**
		 * The advance() function is called by the simulation task. It is used to advance the state
		 * of the scene. This may be the physical state objects in a 3D world are in, or the
		 * simulation itself that the scene performs.
		 */
		virtual void advance(double dt) {}
		
		/**
		 * The render() function is called whenever the engine's render task decides it is time
		 * to update the audio and video presentation of the scene.
		 */
		virtual void render() = 0;
		
		/**
		 * Visual convenience. The following to functions provide an easy way to update the view-
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
		virtual void updateViewport();
		virtual void updateProjection();
		
		virtual bool eventVideoModeChanged();
		
		/**
		 * Notifications. Very useful are the moveOnScreen and moveOffScreen notifications which are
		 * called by the engine whenever a scene is move to or from the screen. They are a good
		 * place to enable/disable the desired OpenGL states.
		 *
		 * By default. the moveOnScreen notification simply updates the viewport and projection.
		 */
		virtual void onMoveOnScreen();
		virtual void onMoveOffScreen() {}
	};
}


#endif
