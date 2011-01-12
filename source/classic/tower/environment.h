#ifndef OSS_CLASSIC_TOWER_ENVIRONMENT_H
#define OSS_CLASSIC_TOWER_ENVIRONMENT_H

#include "../external.h"
#include "../responder.h"


namespace OSS {
	namespace Classic {
		class Tower;
		
		class TowerEnvironment : public Responder {
			
			/**
			 * Construction
			 */
		public:
			const Pointer<Tower> tower;
			
			TowerEnvironment(Tower * tower);
			
			
			/**
			 * Rating
			 */
		private:
			unsigned short rating;
			
		public:
			unsigned short getRating();
			void setRating(unsigned short r);
			
			unsigned int getLobbyStyle();
			
			
			/**
			 * Population
			 */
		private:
			unsigned int population;
			
		public:
			unsigned int getPopulation();
			void setPopulation(unsigned int p);
			
			
			/**
			 * State
			 */
		public:
			virtual void update();
			virtual void updatePopulation();
			
			Core::Updatable::Conditional<TowerEnvironment> updatePopulationIfNeeded;
		};
	}
}


#endif
