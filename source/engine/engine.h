#ifndef OSS_ENGINE_ENGINE_H
#define OSS_ENGINE_ENGINE_H

//Inheritance
#include "external.h"
#include "base/object.h"
#include "base/singleton.h"
#include "basicresponder.h"

//Usage
#include "application.h"
#include "base/pointer.h"


namespace OSS {
	class AbstractStore;
	class Audio;
	class CruiseControl;
	class Scene;
	class Video;
	
	class Engine : public Object, public Singleton<Engine>, public BasicResponder {
		
		/**
		 * Initialization
		 */
	public:
		const Pointer<Application> application;
		Engine(Application * application);
		
		
		/**
		 * Subsystems
		 *
		 * The engine maintains the audio and video singletons, as well as the other subsystems.
		 * The heartbeat() function should be called periodically to advance the engine.
		 */
	private:
		Pointer<CruiseControl> timing;
		
	public:
		Pointer<Audio> audio;
		Pointer<Video> video;
		
	public:
		void heartbeat();
		
		
		/**
		 * Timing
		 */
	public:
		double getTimeElapsed();
		void sleep(double seconds);
		
		
		/**
		 * Scene
		 */
	private:
		Pointer<Scene> scene;
		
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
		 * Events
		 */
	public:
		virtual bool sendEventToNextResponders(Event * event);
	};
}


#include "audio/audio.h"
#include "subsystems/cruisecontrol.h"
#include "scene.h"
#include "video/video.h"


#endif
