#ifndef OSS_WINDOW_H
#define OSS_WINDOW_H

#include "../../general.h"
#include "../../base/base.h"
#include "../../core/sprite.h"


namespace OSS {
	class Window : public Object {
	private:
		recti frame;
	public:
		Sprite backgroundSprite;
		
		//Accessors
	public:
		void setFrame(recti frame);
		recti getFrame();
		
		virtual void draw(rectd visibleRect);
	};
}


#endif
