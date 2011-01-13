#ifndef OSS_CLASSIC_UI_TOOLBOX_WINDOW_H
#define OSS_CLASSIC_UI_TOOLBOX_WINDOW_H

#include "../../external.h"

#include "groupbutton.h"
#include "../../tower/tower.h"


namespace OSS {
	namespace Classic {
		class GameUI;
		
		class ToolsWindow : public GUI::View {
			
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
			std::map< ItemGroup, Pointer<ToolboxGroupButton> > groupButtons;
			
			
			/**
			 * State
			 */
		public:
			virtual void update();
			virtual void layoutSubviews();
			
			Core::Updatable::Conditional<ToolsWindow> layoutSubviewsIfNeeded;
			
			
			/**
			 * Drawing
			 */			
		public:
			virtual void draw(rectd dirtyRect);
		};
	}
}


#endif
