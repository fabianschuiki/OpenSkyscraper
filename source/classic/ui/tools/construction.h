#ifndef OSS_CLASSIC_UI_TOOLS_CONSTRUCTION_H
#define OSS_CLASSIC_UI_TOOLS_CONSTRUCTION_H

#include "../../external.h"

#include "../../items/itemdescriptor.h"
#include "tool.h"


namespace OSS {
	namespace Classic {
		class ConstructionTool : public Tool {
			
			/**
			 * Construction
			 */
		public:
			ConstructionTool(ToolsUI * ui);
			
			
			/**
			 * Item
			 */
		private:
			ItemType itemType;
			ItemDescriptor * itemDescriptor;
			
		public:
			ItemType getItemType();
			void setItemType(ItemType type);
			
			ItemDescriptor * getItemDescriptor();
			void setItemDescriptor(ItemDescriptor * descriptor);
			
			
			/**
			 * Template
			 */
		private:
			recti previousTemplateRect;
			recti templateRect;
			
			double2 templateCenter;
			
		public:
			double2 getTemplateCenter();
			void setTemplateCenter(double2 center);
			
			
			
			/**
			 * State
			 */
		public:
			virtual void update();
			virtual void updateTemplateRect();
			
			Core::Updatable::Conditional<ConstructionTool> updateTemplateRectIfNeeded;
			
			
			/**
			 * Drawing
			 */
		public:
			virtual void draw(rectd dirtyRect);
			
			
			/**
			 * Event Handling
			 */
		public:
			virtual bool eventMouseDown(Core::MouseButtonEvent * event);
			virtual bool eventMouseUp(Core::MouseButtonEvent * event);
			virtual bool eventMouseMoved(Core::MouseMovedEvent * event);
		};
	}
}


#endif
