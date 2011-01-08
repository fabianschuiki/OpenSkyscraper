#ifndef OSS_CLASSIC_TOWER_BACKGROUND_H
#define OSS_CLASSIC_TOWER_BACKGROUND_H

#include "../external.h"


namespace OSS {
	namespace Classic {
		class Tower;
		
		class TowerBackground : public Core::Object {
			
			/**
			 * Construction
			 */
		public:
			const Pointer<Tower> tower;
			
			TowerBackground(Tower * tower);
			
			
			/**
			 * State
			 */
		private:
			bool needsUpdateSkyTextures;
			
		public:
			virtual void update();
			virtual void updateSky();
			virtual void updateSkyTextures();
			
			void updateSkyTexturesIfNeeded();
			void setNeedsUpdateSkyTextures();
			
			
			/**
			 * Simulation
			 */
		private:
			double rainAnimation;
			
		public:
			double getRainAnimation();
			void setRainAnimation(double animation);
			
			virtual void advance(EngineCore * engine, double dt);
			
			
			/**
			 * Sky
			 */
		private:
			Pointer<Texture> currentSkyTextures[10];
			Pointer<Texture> targetSkyTextures[10];
			
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
			
		public:
			std::string getSkyTextureName(unsigned int index, std::string variant);
			
			double getSkyInterpolation();
			void setSkyInterpolation(double f);
			
			SkyState getSkyState();
			void setSkyState(SkyState state);
			
			void setSky(SkyState state, double interpolation);
		};
	}
}


#endif
