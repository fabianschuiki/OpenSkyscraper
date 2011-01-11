#ifndef OSS_ENGINE_SIMULATABLE_H
#define OSS_ENGINE_SIMULATABLE_H


namespace OSS {
	namespace Engine {
		class Scene;
		
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
