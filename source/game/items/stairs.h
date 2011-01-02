#ifndef OSS_STAIRS_H
#define OSS_STAIRS_H


#include "../../general.h"
#include "../item.h"


namespace OSS {
	class StairsItem : public Item {
	public:
		static Item::Descriptor descriptor;
		
		//Initialization
		StairsItem(Tower * tower);
		
		//Animation
	private:
		unsigned int animationFrame;
		double animationProgress;
		bool inUse;
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
