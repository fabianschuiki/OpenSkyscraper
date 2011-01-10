#ifndef OSS_ENGINE_DRAWABLE_H
#define OSS_ENGINE_DRAWABLE_H


namespace OSS {
	namespace Engine {		
		class Scene;
		
		class Drawable {
			
			/**
			 * Drawing
			 */
		public:
			virtual void draw() {}
			virtual void draw(rectd dirtyRect) { draw(); }
			virtual void draw(Scene * scene, rectd dirtyRect) { draw(dirtyRect); }
		};
	}
}


#endif
