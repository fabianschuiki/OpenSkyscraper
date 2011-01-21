#ifndef OSS_GAME_UI_WINDOW_H
#define OSS_GAME_UI_WINDOW_H

#include "view.h"


namespace OSS {
	class Window : public View {
	public:
		virtual void draw(rectd dirtyRect);
	};
}


#endif
