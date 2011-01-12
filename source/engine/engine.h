#ifndef OSS_ENGINE_ENGINE_H
#define OSS_ENGINE_ENGINE_H

#include "external.h"

#include "cruisecontrol.h"


namespace OSS {
	namespace Engine {
		class AbstractStore;
		class Application;
		class Scene;
		
		class EngineCore : public Singleton<EngineCore, Core::Responder> {
			
			/**
			 * Construction
			 */
		public:
			EngineCore(Application * application);
			~EngineCore();
			
			
			/**
			 * Run Loop Attachment
			 */
		private:
			Base::Pointer< Core::Invocation<EngineCore> > invocation;
			
		public:
			Base::Pointer<Application> application;
			
			void attachToApplication(Application * app);
			void detachFromApplication();
			
			virtual void willAttachToApplication(Application * application) {}
			virtual void didAttachToApplication() {}
			
			
			/**
			 * State
			 */
		public:
			virtual void update();
			
			
			/**
			 * Timing
			 */
		public:
			Base::Pointer<CruiseControl> timing;
			
			
			/**
			 * Scene
			 */
		private:
			Base::Pointer<Scene> scene;
			
		public:
			Scene * getScene();
			void setScene(Scene * scene);
			
			virtual void willSwitchToScene(Scene * scene) {}
			virtual void didSwitchToScene(Scene * scene) {}
			
			void simulateScene();
			void updateScene();
			void drawScene();
			
			
			/**
			 * Stores
			 *
			 * We need to keep a memory-managed list of stores somewhere for them to work properly.
			 * The stores concept is pretty bad software design and should be replaced sometime by
			 * something that actually works :D.
			 */
		private:
			typedef vector< Pointer<AbstractStore> > StoreList;
			StoreList stores;
			
		public:
			void performLoadingAndFinalizing();
			
			
			/**
			 * Event Sending
			 */
		public:
			virtual bool sendEventToNextResponders(Base::Event * event);
		};
	}
}


#endif
