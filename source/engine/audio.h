#ifndef OSS_ENGINE_AUDIO_H
#define OSS_ENGINE_AUDIO_H

#include "../core~/responder.h"

#include "../core~/application.h"


namespace OSS {
	namespace Engine {
		class Audio : public Core::Responder {
			
			/**
			 * Construction
			 */
		public:
			const Base::Pointer<Core::Application> application;
			
			Audio(Core::Application * application);
			~Audio();
		};
	}
}


#endif
