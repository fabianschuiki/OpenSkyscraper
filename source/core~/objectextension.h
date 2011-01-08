#ifndef OSS_CORE_OBJECTEXTENSION_H
#define OSS_CORE_OBJECTEXTENSION_H


namespace OSS {
	namespace Core {
		class ObjectExtension {
			
			/**
			 * State
			 *
			 * Functionality to initialize and update an object's internal state.
			 */
		public:
			virtual void init() {}
			virtual void update() {}
		};
	}
}


#endif
