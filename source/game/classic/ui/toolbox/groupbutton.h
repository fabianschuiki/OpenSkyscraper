#ifndef OSS_CLASSIC_UI_TOOLBOX_GROUPBUTTON_H
#define OSS_CLASSIC_UI_TOOLBOX_GROUPBUTTON_H

#include "window.h"

#include "../../items/itemdescriptor.h"


namespace OSS {
	namespace Classic {		
		//TODO: change to subclass of GUI::Button as soon as it's written.
		class ToolsGroupButton : public View {
			
			/**
			 * Construction
			 */
		public:
			const Pointer<ToolsWindow> window;
			const ItemGroup group;
			
			ToolsGroupButton(ToolsWindow * window, ItemGroup group);
			
			
			/**
			 * Drawing
			 */			
		public:
			virtual void draw(rectd dirtyRect);
			
			
			//Add some popup view.
			//Add list of buttons, each corresponding to an item in this group.
			//Add methods to calculate which buttons are visible and which aren't based on tower rating.
			//Update button visibility when tower rating changes (event).
			//Add layout functions that position the buttons in the popup view.
		};
	}
}


#endif
