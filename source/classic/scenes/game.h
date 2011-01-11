#ifndef OSS_CLASSIC_SCENES_GAME_H
#define OSS_CLASSIC_SCENES_GAME_H

#include "../external.h"

#include "../tower/tower.h"
#include "../items/item.h"


namespace OSS {
	namespace Classic {
		//class GUI;
		
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
			//Pointer<GUI> gui;
			
			
			
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
			 * State
			 */
		public:
			virtual void update();
			
			
			/**
			 * Drawing
			 */
		public:
			virtual void draw();
			
			
			/**
			 * Presentation
			 */
		public:
			virtual void didMoveOnScreen();
			virtual void willMoveOffScreen();
			
			
			/**
			 * Event Sending
			 */
		public:
			virtual bool sendEventToNextResponders(Base::Event * event);
			
			
			/**
			 * Event Handling
			 */
		public:
			virtual bool eventScrollWheel(Core::ScrollWheelEvent * event);
		};
	}
}


#endif
