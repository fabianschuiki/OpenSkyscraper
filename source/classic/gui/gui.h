#ifndef OSS_CLASSIC_GUI_GUI_H
#define OSS_CLASSIC_GUI_GUI_H

#include "../external.h"


namespace OSS {
	namespace Classic {
		class GameScene;
		
		class GUI : public Responder, public Engine::Drawable, public Engine::Simulatable {
			
			/**
			 * Construction
			 */
		public:
			const Pointer<GameScene> scene;
			
			GUI(GameScene * scene);
			
			
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
		};
	}
}


#endif
