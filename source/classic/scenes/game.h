#ifndef OSS_CLASSIC_SCENES_GAME_H
#define OSS_CLASSIC_SCENES_GAME_H

#include "../external.h"

#include "../tower/tower.h"
#include "../items/item.h"


namespace OSS {
	namespace Classic {
		class GUI;
		
		class GameScene : public Engine::Scene {
			/**
			 * Construction
			 */
		public:
			const Pointer<Tower> tower;
			
			GameScene(Tower * tower, Engine::EngineCore * engine);
			
			
			/**
			 * GUI
			 */
		private:
			Pointer<GUI> gui;
			
			
			
			//Point of interest, i.e. the point in the world that's centered on-screen
			double2 POI;
			
			//Mouse location inside the world
			double2 previousWorldMouse;
			double2 worldMouse;
			bool mouseMoved;
			
			//Calculated culling values
			rectd visibleRect;
			recti visibleCells;
			
			//Visible items
			std::set<unsigned int> visibleFacilities;
			std::set<unsigned int> visibleTransports;
			
			
			/**
			 * Simulation
			 */
		public:
			virtual void advance(double dt);
			
			
			/**
			 * Drawing
			 */
		public:
			virtual void draw(rectd dirtyRect);
			void drawBackground(rectd dirtyRect);
			void drawFacilities(rectd dirtyRect);
			void drawTransports(rectd dirtyRect);
			void drawGUI(rectd dirtyRect);
			
			/**
			 * Presentation
			 */
		public:
			virtual void didMoveOnScreen();
			virtual void willMoveOffScreen();
			
			
			/**
			 * Construction
			 */		
		public:
			Item::Descriptor * constructionItemDescriptor;
			recti constructionTemplate;
			
			bool isDraggingConstruction;
			recti previousConstructionTemplate;
			
			void updateConstruction();
			void startConstruction();
			void endConstruction();
			
			
			/**
			 * Uncategorized
			 */		
		private:
			ToolboxWindow::Tool tool;
			
		public:
			ToolboxWindow::Tool getTool();
			void setTool(ToolboxWindow::Tool tool);
			void setConstructionTool(Item::Type itemType);
			
			ToolboxWindow toolboxWindow;
			ControlWindow controlWindow;
			Engine::Sprite debugConstructionToolSprite;
			unsigned char debugItemType;
			
			virtual void eventPrepare();
			
			void buildDebugTower();
		};
	}
}


#endif
