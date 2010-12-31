#ifndef OSS_ENGINE_H
#define OSS_ENGINE_H

#include "../general.h"
#include "../base/base.h"
#include "coreobject.h"

#include "tasks/tasks.h"


namespace OSS {	
	enum EngineState {
		kEngineLoadingState,
		kEngineSimulationState
	};
	
	class Engine : public Singleton<Engine, CoreObject> {
		
	public:
		/**
		 * Application
		 *
		 * The engine keeps a reference to the application instanced which created it.
		 */
		
		/**
		 * Tasks
		 *
		 * The input task is responsible for polling SDL events, wrapping them up in the engine's
		 * own Events and sending them down the pipe. The simulation task advances the state of the
		 * current scene. The render task is responsible for rendering the current scene. This means
		 * redrawing its video output and updating any audio output to match the scene's current
		 * state. The loader task works down the loading and unloading queues of the various stores.
		 */
		friend class InputTask;
		friend class SimulationTask;
		friend class AudioTask;
		friend class RenderTask;
		friend class LoaderTask;
		
		InputTask inputTask;
		SimulationTask simulationTask;
		AudioTask audioTask;
		RenderTask renderTask;
		LoaderTask loaderTask;
		
		
		/**
		 * Scene
		 *
		 * The engine can be assigned a scene which is used to generate visual and aural content
		 * for the user. The scene is fed with input, advanced in state and updated by the different
		 * tasks of the engine. Note that the "scene" variable is protected. You must use the
		 * "switchToScene" function to change the scene. This is due to the fact that the scenes
		 * themselves need to be updated and given a chance to adjust to the new on-/offscreen
		 * state.
		 */
	protected:
		Pointer<Scene> scene;
	public:
		void switchToScene(Scene * newScene);
		
		
		//Initialization
		Engine();
		~Engine();
		
		//Runloop
		void runloopCycle();
		
		//Events
		bool handleEvent(CoreEvent * event);
		
		/**
		 * Event Handlers
		 */
		
		//SDL
		bool eventSDL(SDL_Event * event);
		
		//Video Mode
		void eventVideoModeChanged();
		
		
	protected:
		/**
		 * Timing
		 *
		 * Calling the timingFrameStart() function will start the time-taking for the
		 * current frame. The frame's dt (time elapse) is defined as the time offset
		 * between two consequent calls to timingFrameStart(). In order to capture
		 * any OpenGL-induced waiting states, calling timingRenderingDone() will mark
		 * the point in time where the rendering was done and the "control" is handed
		 * over to OpenGL to swap the buffers.
		 */
		
		//Calculated timing characteristics
		double dt; //frame delta time, in seconds
		double freq; //frame frequency, in Hertz
		double idle_dt; //idle time while waiting for buffer swap, in seconds
		double idle_ratio; //idle time to frame delta time ratio
		
		//Frame times
		double previous_frameTime;
		double current_frameTime;
		double renderingDone_frameTime;
		
		//Enforced timing limits
		double freq_lowerLimit;
		double freq_upperLimit;
		bool lowerFrequencyLimitEnforced;	//set to true if the lower limit was enforced,
											//therefore slowing down the simulation as the
											//CPU can't keep up.
		bool upperFrequencyLimitEnforced;	//set to true if the upper limit was enforced,
											//i.e. the CPU is capable of a higher framerate
											//and the runloop had to be slowed down.
		
		void timingFrameStart();
		void timingRenderingDone();
		
		
		/**
		 * Uncategorized
		 */
	public:
		typedef std::vector< Pointer<AbstractStore> > StoreList;
		StoreList stores;
	};
}

#endif
