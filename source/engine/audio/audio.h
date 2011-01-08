#ifndef OSS_ENGINE_AUDIO_H
#define OSS_ENGINE_AUDIO_H

#include "../../core/responder.h"

#include "../../core/application.h"

//OpenAL
#ifdef __APPLE__
#include <OpenAL/al.h>
//#include <OpenAL/alc.h>
#include "../../os/macosx/alc.h" //required since alc.h in OS X uses typedef'd void for empty param list -> C++ sucks!
#endif


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
