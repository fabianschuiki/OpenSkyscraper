#ifndef OSS_ENGINE_SIMULATABLE_H
#define OSS_ENGINE_SIMULATABLE_H

#include "scene.h"


namespace OSS {
	namespace Engine {
		class Simulatable {
			
			/**
			 * Simulation
			 */
		public:
			virtual void advance(double dt) {}
			virtual void advance(Scene * scene, double dt) { advance(dt); }
		};
	}
}


#endif
