#ifndef OSS_ENGINE_SIMULATABLE_H
#define OSS_ENGINE_SIMULATABLE_H


namespace OSS {
	namespace Engine {
		class Simulatable {
			
			/**
			 * Simulation
			 */
		public:
			virtual void advance(double dt) {}
		};
	}
}


#endif
