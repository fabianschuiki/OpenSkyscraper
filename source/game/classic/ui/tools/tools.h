#ifndef OSS_CLASSIC_UI_TOOLS_TOOLS_H
#define OSS_CLASSIC_UI_TOOLS_TOOLS_H

#include "../game.h"


namespace OSS {
	namespace Classic {
		class ConstructionTool;
		class Tool;
		
		class ToolsUI : public GameObject {
			
			/**
			 * Construction
			 */
		public:
			const Pointer<GameUI> ui;
			
			ToolsUI(GameUI * ui);
			
			Tower * getTower();
			GameScene * getScene();
			
			
			/**
			 * Tools
			 */
		private:
			Pointer<Tool> tool;
			
		public:
			Tool * getTool();
			void setTool(Tool * tool);
			
			Pointer<ConstructionTool> constructionTool;
			//Pointer<BulldozerTool> bulldozerTool;
			//Pointer<FingerTool> fingerTool;
			//Pointer<InspectionTool> inspectionTool;
			
			void selectItemConstructionTool(ItemType type);
			
			
			/**
			 * Engine Object
			 */
		public:
			virtual void update();
			virtual void draw(rectd dirtyRect);
			
			
			/**
			 * Event Sending
			 */
		public:
			virtual bool sendEventToNextResponders(OSS::Event * event);
		};
	}
}


#include "tool.h"
#include "construction.h"


#endif
