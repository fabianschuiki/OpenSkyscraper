#ifndef OSS_CLASSIC_UI_TOOLS_TOOLS_H
#define OSS_CLASSIC_UI_TOOLS_TOOLS_H

#include "../../external.h"
#include "../../responder.h"

#include "construction.h"
#include "tool.h"


namespace OSS {
	namespace Classic {
		class GameScene;
		class GameUI;
		class Tower;
		
		class ToolsUI : public Responder {
			
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
			virtual bool sendEventToNextResponders(Base::Event * event);
		};
	}
}


#endif
