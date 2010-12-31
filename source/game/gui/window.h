#ifndef OSS_WINDOW_H
#define OSS_WINDOW_H

#include "../../general.h"
#include "../../base/base.h"
#include "../../core/sprite.h"


namespace OSS {
	class Window : public CoreObject {
	public:
		//Initialization
		Window();
		
		//Background
		Sprite backgroundSprite;
		
		//Frame
	private:
		recti frame;
	public:
		void setFrame(recti frame);
		recti getFrame();
		
		//Border
		color4d borderColor;
		
		//Layout
		void eventVideoModeChanged();
		virtual void layout() {}
		
		//Rendering
		virtual void draw(rectd visibleRect);
	};
}


#endif
