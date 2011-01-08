#ifndef OSS_ENGINE_CRUISECONTROL_H
#define OSS_ENGINE_CRUISECONTROL_H

#include "../base/object~.h"

#include "../base/pointer.h"


namespace OSS {
	namespace Engine {
		class EngineCore;
		
		class CruiseControl : public Base::Object {
			
			/**
			 * Construction
			 */
		public:
			const Base::Pointer<EngineCore> engine;
			CruiseControl(EngineCore * engine);
			
			
			/**
			 * Timing
			 *
			 * Calling the frameStart() function will start the time-taking for the current frame.
			 * The frame's dt (time elapse) is defined as the time offset between two consequent
			 * calls to timingFrameStart(). In order to capture any OpenGL-induced waiting states,
			 * calling timingRenderingDone() will mark the point in time where the rendering was
			 * done and the "control" is handed over to OpenGL to swap the buffers.
			 */
			
			//Calculated timing characteristics
		public:
			double dt; //frame delta time, in seconds
			double freq; //frame frequency, in Hertz
			double idle_dt; //idle time while waiting for buffer swap, in seconds
			double idle_ratio; //idle time to frame delta time ratio
			
			//Same as above, but suited for output to the user
		public:
			double damped_dt;
			double damped_freq;
			double damped_idle_dt;
			double damped_idle_ratio;
			
			//Frame times
		private:
			double previous_frameTime;
			double current_frameTime;
			double renderingDone_frameTime;
			
			//Enforced timing limits
		public:
			double freq_lowerLimit;
			double freq_upperLimit;
			bool lowerFrequencyLimitEnforced;	//set to true if the lower limit was enforced,
												//therefore slowing down the simulation as the
												//CPU can't keep up.
			bool upperFrequencyLimitEnforced;	//set to true if the upper limit was enforced,
												//i.e. the CPU is capable of a higher framerate
												//and the runloop had to be slowed down.
			
			void frameStart();
			void renderingDone();
		};
	}
}


#endif
