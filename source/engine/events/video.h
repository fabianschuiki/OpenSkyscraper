#ifndef OSS_ENGINE_EVENTS_VIDEO_H
#define OSS_ENGINE_EVENTS_VIDEO_H

#include "../event.h"

#include "../video.h"
#include "../../base/pointer.h"


namespace OSS {
	namespace Engine {		
		class VideoEvent : public Event {
		public:
			const Base::Pointer<Video> video;
			
			VideoEvent(Event::Type type, Video * video)
			: Event(type), video(video) {}
		};
	}
}


#endif
