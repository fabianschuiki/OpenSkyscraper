#ifndef OSS_GAME_CLASSIC_TOWER_BACKGROUND_H
#define OSS_GAME_CLASSIC_TOWER_BACKGROUND_H

#include "tower.h"


namespace OSS {
	namespace Classic {
		class TowerBackground : public SceneObject, public Responder {
			
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
			
			Updatable::Conditional<TowerBackground> updateSkyIfNeeded;
			Updatable::Conditional<TowerBackground> updateSkyTexturesIfNeeded;
			Updatable::Conditional<TowerBackground> updateGroundTexturesIfNeeded;
			
			
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
			Pointer<Texture> currentSkyTextures[10];
			Pointer<Texture> targetSkyTextures[10];
			
			Pointer<Texture> currentCloudTextures[6];
			Pointer<Texture> targetCloudTextures[6];
			
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
			Pointer<Texture> cityTexture;
			Pointer<Texture> groundTexture;
			
		public:
			string getCityTextureName();
			string getGroundTextureName();
			
			
			/**
			 * Sound Effects
			 */
		private:
			Pointer<SoundEffect> cockSound;
			Pointer<SoundEffect> birdsMorningSound;
			Pointer<SoundEffect> bellsAt10Sound;
			Pointer<SoundEffect> birdsEveningSound;
			Pointer<SoundEffect> rainSound;
			Pointer<SoundEffect> thunderSound;
			
			double nextThunderCountdown;
		};
	}
}


#endif
