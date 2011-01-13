#ifndef OSS_CLASSIC_UI_TOOLBOX_WINDOW_H
#define OSS_CLASSIC_UI_TOOLBOX_WINDOW_H

#include "../game.h"


namespace OSS {
	namespace Classic {	
		class ToolsGroupButton;
		
		class ToolsWindow : public View {
			
			/**
			 * Construction
			 */
		public:
			const Pointer<GameUI> ui;
			
			ToolsWindow(GameUI * ui);
			
			//TODO: move this somewhere else
			Tower * getTower();
			
			
			/**
			 * Subviews
			 */
		private:
			map< ItemGroup, Pointer<ToolsGroupButton> > groupButtons;
			
			
			/**
			 * State
			 */
		public:
			virtual void update();
			virtual void layoutSubviews();
			
			Updatable::Conditional<ToolsWindow> layoutSubviewsIfNeeded;
			
			
			/**
			 * Drawing
			 */			
		public:
			virtual void draw(rectd dirtyRect);
		};
	}
}


#include "groupbutton.h"


#endif
