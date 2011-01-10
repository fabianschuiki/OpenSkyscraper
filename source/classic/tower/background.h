#ifndef OSS_CLASSIC_TOWER_BACKGROUND_H
#define OSS_CLASSIC_TOWER_BACKGROUND_H

#include "../external.h"


namespace OSS {
	namespace Classic {
		class Tower;
		
		class TowerBackground : public Engine::Object {
			
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
			
			Conditional<TowerBackground> updateSkyTexturesIfNeeded;
			Conditional<TowerBackground> updateGroundTexturesIfNeeded;
			
			
			/**
			 * Drawing
			 */
		public:
			virtual void draw(rectd dirtyRect);
			virtual void drawSky(rectd dirtyRect);
			virtual void drawGround(rectd dirtyRect);
			
			
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
			std::string getSkyTextureName(unsigned int index, std::string variant);
			std::string getCloudTextureName(unsigned int index, std::string variant);
			
			double getSkyInterpolation();
			void setSkyInterpolation(double f);
			
			SkyState getSkyState();
			void setSkyState(SkyState state);
			
			void setSky(SkyState state, double interpolation);
			
			unsigned int getRainAnimationFrame();
			void setRainAnimationFrame(unsigned int frame);
			
			bool hasCloudAt(double2 location);
			double cloudNoise(double x, double y);
			
			
			/**
			 * Ground
			 */
		private:
			Pointer<Engine::Texture> cityTexture;
			Pointer<Engine::Texture> groundTexture;
			
		public:
			std::string getCityTextureName();
			std::string getGroundTextureName();
		};
	}
}


#endif
