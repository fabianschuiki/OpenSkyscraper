#ifndef OSS_CLASSIC_ITEMS_TRANSPORTS_STAIRSLIKE_STAIRSLIKE_H
#define OSS_CLASSIC_ITEMS_TRANSPORTS_STAIRSLIKE_STAIRSLIKE_H

#include "../transport.h"


namespace OSS {
	namespace Classic {
		class StairslikeItem : public TransportItem {
		public:
			//Initialization
			StairslikeItem(Tower * tower, ItemDescriptor * descriptor);
			
			//Connection
			std::set<int> getConnectionFloors();
			
			//Animation
		private:
			unsigned int animationFrame;
			double animationProgress;
			bool inUse;
		protected:
			unsigned int numAnimationFrames;
			unsigned int numAnimationFramesPerTexture;
			std::string baseTextureName;
		public:
			unsigned int getAnimationFrame();
			void setAnimationFrame(unsigned int animationFrame);
			double getAnimationProgress();
			void setAnimationProgress(double animationProgress);
			bool isInUse();
			void setInUse(bool inUse);
			
			//Basic Sprites
			void initBackground();
			void updateBackground();
			
			//Simulation
			void advance(double dt);
			
			//People
		private:
			typedef std::map< Person *, double > TransitProgressMap;
			TransitProgressMap transitProgress;
		public:
			void addPerson(Person * person);
			void removePerson(Person * person);
		};
	}
}


#endif 
