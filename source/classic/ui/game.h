#ifndef OSS_CLASSIC_UI_GAME_H
#define OSS_CLASSIC_UI_GAME_H

#include "../external.h"
#include "../responder.h"

#include "control/window.h"
#include "../tower/tower.h"


namespace OSS {
	namespace Classic {
		class GameScene;
		
		class GameUI : public Responder {
			
			/**
			 * Construction
			 */
		public:
			const Pointer<GameScene> scene;
			
			GameUI(GameScene * scene);
			
			Tower * getTower();
			
			
			/**
			 * View
			 */
		private:
			Pointer<GUI::View> rootView;
			
			
			/**
			 * Windows
			 */
		private:
			Pointer<ControlWindow> controlWindow;
			//Pointer<MapWindow> mapWindow;
			//Pointer<ToolboxWindow> toolboxWindow;
			
			
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
			virtual void updateRootView();
			
			Core::Updatable::Conditional<GameUI> updateRootViewIfNeeded;
			
			
			/**
			 * Drawing
			 */
		public:
			virtual void draw(rectd dirtyRect);
			
			
			/**
			 * Event Handling
			 */
		public:
			virtual void eventVideoChanged(Engine::VideoEvent * event);
		};
	}
}


#endif
