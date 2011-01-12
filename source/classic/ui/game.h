#ifndef OSS_CLASSIC_UI_GAME_H
#define OSS_CLASSIC_UI_GAME_H

#include "../external.h"
#include "../responder.h"

#include "control/window.h"
#include "../tower/tower.h"
#include "tools/tools.h"


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
			
			//TODO: move this somewhere else
			Tower * getTower();
			
			
			/**
			 * Subsystems
			 */
		private:
			Pointer<GUI::View> rootView;
			Pointer<ToolsUI> tools;
			
			
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
			 * Event Sending
			 */
		public:
			virtual bool sendEventToNextResponders(Base::Event * event);
			
			
			/**
			 * Event Handling
			 */
		public:
			virtual void eventVideoChanged(Engine::VideoEvent * event);
		};
	}
}


#endif
