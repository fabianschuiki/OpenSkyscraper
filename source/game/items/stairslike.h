#ifndef OSS_STAIRSLIKE_H
#define OSS_STAIRSLIKE_H


#include "../../general.h"
#include "../transportitem.h"


namespace OSS {
	class StairslikeItem : public TransportItem {
	public:
		//Initialization
		StairslikeItem(Tower * tower, Descriptor * descriptor);
		
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
	};
}


#endif 
