#ifndef OSS_CLASSIC_UI_TOOLBOX_GROUPBUTTON_H
#define OSS_CLASSIC_UI_TOOLBOX_GROUPBUTTON_H

#include "../button.h"

#include "window.h"
#include "../../items/itemdescriptor.h"


namespace OSS {
	namespace Classic {		
		//TODO: change to subclass of GUI::Button as soon as it's written.
		class ToolsGroupButton : public Button {
			
			/**
			 * Construction
			 */
		public:
			const Pointer<ToolsWindow> window;
			const ItemGroup group;
			
			ToolsGroupButton(ToolsWindow * window, ItemGroup group);
			
			
			/**
			 * PopUp
			 */
		private:
			typedef map<ItemType, Pointer<Button> > ItemButtonMap;
			
			Pointer<View> popup;
			ItemButtonMap itemButtons;
			ItemType selectedItem;
			
			double2 popupOffset;
			
		public:
			ItemType getSelectedItem();
			void setSelectedItem(ItemType type);
			
			Button * getSelectedButton();
			
			
			/**
			 * State
			 */
		public:
			virtual void update();
			virtual void updatePopUp();
			
			
			/**
			 * Events
			 */
		private:
			bool trackingMouse;
			
		public:
			virtual bool eventMouseDown(MouseButtonEvent * event);
			virtual bool eventMouseMove(MouseMoveEvent * event);
			virtual bool eventMouseUp(MouseButtonEvent * event);
			
			void highlightPopUpButtonUsingEvent(MouseEvent * event);
			
			
			//Add some popup view.
			//Add list of buttons, each corresponding to an item in this group.
			//Add methods to calculate which buttons are visible and which aren't based on tower rating.
			//Update button visibility when tower rating changes (event).
			//Add layout functions that position the buttons in the popup view.
		};
	}
}


#endif
