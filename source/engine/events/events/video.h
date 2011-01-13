#ifndef OSS_ENGINE_EVENTS_VIDEO_H
#define OSS_ENGINE_EVENTS_VIDEO_H

#include "../../external.h"
#include "../event.h"


namespace OSS {
	class Video;
	
	class VideoEvent : public Event {
	public:
		const Pointer<Video> video;
		
		VideoEvent(Event::Type type, Video * video)
		: Event(type), video(video) {}
	};
}


#include "../../video/video.h"


#endif
