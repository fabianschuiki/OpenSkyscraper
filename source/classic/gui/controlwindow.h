#ifndef OSS_CONTROLWINDOW_H
#define OSS_CONTROLWINDOW_H

#include "../../general.h"
#include "window.h"
#include "../tower.h"


namespace OSS {
	class ControlWindow : public Window {
	public:
		Pointer<Tower> tower;
		
		//Initialization
		ControlWindow();
		
		//Sprites
	private:
		unsigned short visibleStarRating; //for caching
		Sprite starSprite[5];
		Sprite towerRatingSprite;
		
		//Layout
	public:
		void layout();
		
		//Rendering
		void draw(rectd visibleRect);
		void drawWatch(rectd visibleRect);
		void drawRating(rectd visibleRect);
	};
}


#endif
