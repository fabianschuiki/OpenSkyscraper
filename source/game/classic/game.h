#ifndef OSS_GAME_CLASSIC_SCENES_GAME_H
#define OSS_GAME_CLASSIC_SCENES_GAME_H

#include "../external.h"
#include "events/responder.h"

#include "items/itemdescriptor.h"
#include "gameobject.h"


namespace OSS {
	namespace Classic {
		class GameUI;
		class Tower;
		
		class GameScene : public Scene, public Responder {
			/**
			 * Construction
			 */
		public:
			const Pointer<Tower> tower;
			
			GameScene(Tower * tower, Engine * engine);
			
			
			/**
			 * UI
			 */
		private:
			Pointer<GameUI> ui;
			
			
			/**
			 * Camera
			 *
			 * Point of interest, i.e. the point in the world that's centered on-screen.
			 */
		private:
			double2 POI;
			rectd visibleRect;
			rectd animatedVisibleRect;
			
		public:
			const double2 & getPOI();
			void setPOI(double2 p);
			
			const rectd & getVisibleRect();
			void setVisibleRect(rectd rect);
			
			double2 windowToWorld(double2 v, bool flipped);
			double2 windowToWorld(double2 v);
			rectd windowToWorld(rectd v, bool flipped);
			rectd windowToWorld(rectd v);
			
			double2 worldToWindow(double2 v, bool flipped);
			double2 worldToWindow(double2 v);
			rectd worldToWindow(rectd v, bool flipped);
			rectd worldToWindow(rectd v);
			
			
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
			
			Updatable::Conditional<GameScene> updateVisibleRectIfNeeded;
			
			
			/**
			 * Drawing
			 */
		public:
			virtual void draw(rectd dirtyRect);
			
			
			/**
			 * Presentation
			 */
		public:
			virtual void didMoveOnScreen();
			virtual void willMoveOffScreen();
			
			
			/**
			 * Events
			 */
		public:
			virtual bool sendEventToNextResponders(OSS::Event * event);
			
			virtual bool eventScrollWheel(ScrollWheelEvent * event);
			virtual bool eventKeyDown(KeyEvent * event);
			
			//DEBUG
			void buildDebugTower();
		};
	}
}


//Include the forward-declared stuff
#include "ui/game.h"
#include "tower/tower.h"


#endif
