#ifndef OSS_CLASSIC_SCENES_GAME_H
#define OSS_CLASSIC_SCENES_GAME_H

#include "../external.h"

#include "../gui/gui.h"
#include "../tower/tower.h"
#include "../items/item.h"


namespace OSS {
	namespace Classic {
		
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
			
			
			/**
			 * Camera
			 *
			 * Point of interest, i.e. the point in the world that's centered on-screen.
			 */
		private:
			double2 POI;
			rectd visibleRect;
			
		public:
			const double2 & getPOI();
			void setPOI(double2 p);
			
			const rectd & getVisibleRect();
			void setVisibleRect(rectd rect);
			
			double2 windowToWorld(double2 v);
			double2 worldToWindow(double2 v);
			
			
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
			virtual void updateVisibleRect();
			
			Core::Updatable::Conditional<GameScene> updateVisibleRectIfNeeded;
			
			
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
