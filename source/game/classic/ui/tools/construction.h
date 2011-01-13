#ifndef OSS_CLASSIC_UI_TOOLS_CONSTRUCTION_H
#define OSS_CLASSIC_UI_TOOLS_CONSTRUCTION_H

#include "tool.h"

#include "tools.h"
#include "../../items/itemdescriptor.h"


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
			recti initialTemplateRect;
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
			
			Updatable::Conditional<ConstructionTool> updateTemplateRectIfNeeded;
			
			
			/**
			 * Drawing
			 */
		public:
			virtual void draw(rectd dirtyRect);
			
			
			/**
			 * Event Handling
			 */
		private:
			bool isDraggingConstruction;
			
		public:
			virtual bool eventMouseDown(MouseButtonEvent * event);
			virtual bool eventMouseUp(MouseButtonEvent * event);
			virtual bool eventMouseMove(MouseMoveEvent * event);
		};
	}
}


#endif
