#ifndef OSS_CLASSIC_TOWER_BACKGROUND_H
#define OSS_CLASSIC_TOWER_BACKGROUND_H

#include "../external.h"
#include "../responder.h"


namespace OSS {
	namespace Classic {
		class Tower;
		
		class TowerBackground : public Responder {
			
			/**
			 * Construction
			 */
		public:
			const Pointer<Tower> tower;
			
			TowerBackground(Tower * tower);
			
			
			/**
			 * Simulation
			 */
		private:
			bool rainyDay;
			double rainAnimation;
			
		public:
			double getRainAnimation();
			void setRainAnimation(double animation);
			
			bool isRainyDay();
			void setRainyDay(bool rainy);
			
			virtual void advance(double dt);
			
			
			/**
			 * State
			 */			
		public:
			virtual void update();
			virtual void updateSky();
			virtual void updateSkyTextures();
			virtual void updateGroundTextures();
			
			Core::Updatable::Conditional<TowerBackground> updateSkyIfNeeded;
			Core::Updatable::Conditional<TowerBackground> updateSkyTexturesIfNeeded;
			Core::Updatable::Conditional<TowerBackground> updateGroundTexturesIfNeeded;
			
			
			/**
			 * Drawing
			 */
		public:
			virtual void draw(rectd dirtyRect);
			virtual void drawSky(rectd dirtyRect);
			virtual void drawGround(rectd dirtyRect);
			
			
			/**
			 * Event Handling
			 */
		public:
			virtual void eventTimeChanged(Event * event);
			virtual void eventDayChanged(Event * event);
			
			
			/**
			 * Sky
			 */
		private:
			Pointer<Engine::Texture> currentSkyTextures[10];
			Pointer<Engine::Texture> targetSkyTextures[10];
			
			Pointer<Engine::Texture> currentCloudTextures[6];
			Pointer<Engine::Texture> targetCloudTextures[6];
			
			double skyInterpolation;
			
			typedef enum {
				Night = 0,
				EarlyDawn,
				LateDawn,
				Day,
				Worsening,
				Rain,
				Improving,
				EarlyDusk,
				LateDusk
			} SkyState;
			SkyState skyState;
			
			unsigned int rainAnimationFrame;
			
		public:
			string getSkyTextureName(unsigned int index, string variant);
			string getCloudTextureName(unsigned int index, string variant);
			
			double getSkyInterpolation();
			void setSkyInterpolation(double f);
			
			SkyState getSkyState();
			void setSkyState(SkyState state);
			
			void setSky(SkyState state, double interpolation);
			
			unsigned int getRainAnimationFrame();
			void setRainAnimationFrame(unsigned int frame);
			
			bool hasCloudAt(double2 location);
			double cloudNoise(double2 p);
			
			
			/**
			 * Ground
			 */
		private:
			Pointer<Engine::Texture> cityTexture;
			Pointer<Engine::Texture> groundTexture;
			
		public:
			string getCityTextureName();
			string getGroundTextureName();
			
			
			/**
			 * Sound Effects
			 */
		private:
			Pointer<Engine::SoundEffect> cockSound;
			Pointer<Engine::SoundEffect> birdsMorningSound;
			Pointer<Engine::SoundEffect> bellsAt10Sound;
			Pointer<Engine::SoundEffect> birdsEveningSound;
			Pointer<Engine::SoundEffect> rainSound;
			Pointer<Engine::SoundEffect> thunderSound;
			
			double nextThunderCountdown;
		};
	}
}


#endif
