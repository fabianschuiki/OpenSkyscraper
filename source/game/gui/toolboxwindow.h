#ifndef OSS_TOOLBOXWINDOW_H
#define OSS_TOOLBOXWINDOW_H

#include "../../general.h"
#include "window.h"
#include "../item.h"


namespace OSS {
	class TowerScene;
	
	class ToolboxWindow : public Window {
	public:
		//Tools
		typedef enum {
			kBulldozerTool,
			kHandTool,
			kInspectTool,
			kConstructionTool
		} Tool;
		
	public:
		const Pointer<TowerScene> towerScene;
		
		//Initialization
		ToolboxWindow(TowerScene * scene);
		
		
		/**
		 * Buttons
		 */
	private:
		Sprite pauseButton;
		bool pauseButtonPressed;
		
		typedef std::map<Tool, Sprite> ToolButtonMap;
		ToolButtonMap toolButtons;
		typedef std::map<Item::Type, Sprite> ItemButtonMap;
		ItemButtonMap itemButtons;
		
		typedef std::vector<Item::Type> ItemTypes;
		typedef std::map<Item::Group, ItemTypes> GroupMap;
		typedef std::map<Item::Group, unsigned int> GroupSelectionMap;
		GroupMap groups;
		GroupSelectionMap selectedGroupIndices;
	public:
		void initButtons();
		void updateButtons();
		
		void initPauseButton();
		void initToolButtons();
		void initItemButtons();
		
		void updatePauseButton();
		void updateToolButtons();
		void updateItemButtons();
		
		
		/**
		 * Layout
		 */
		void layout();
		
		void layoutPauseButton();
		void layoutToolButtons();
		void layoutItemButtons();
		
		
		//Drawing
		void draw(rectd visibleRect);
		
		
		/**
		 * Events
		 */
		bool eventMouseDown(SDL_Event * event);
		bool eventMouseUp(SDL_Event * event);
	};
}


#endif
