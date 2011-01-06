#ifndef OSS_ELEVATOR_H
#define OSS_ELEVATOR_H


#include "../../general.h"
#include "../transportitem.h"
#include "elevatorbackground.h"


namespace OSS {
	class ElevatorItem : public TransportItem {
	public:
		//Initialization
		ElevatorItem(Tower * tower, Descriptor * descriptor);
		
		//Basic Sprites
		void initBasicSprites();
		void updateBasicSprites();
		
		//Background
	private:
		ElevatorBackground background;
	public:
		void initBackground();
		bool isFloorActive(int floor);
		bool isFloorHighlighted(int floor);
		
		
		/**
		 * Motor and Buffer
		 */
	private:
		Sprite motorSprite;
		Sprite bufferSprite;
	public:
		void initMotorBuffer();
		void updateMotorBuffer();
		
		//Animation
	private:
		bool animateMotorBuffer;
		unsigned int motorbufferAnimationIndex;
		double motorbufferAnimation;
		
	public:
		bool getAnimateMotorBuffer();
		void setAnimateMotorBuffer(bool animate);
		
		unsigned int getMotorbufferAnimationIndex();
		void setMotorbufferAnimationIndex(unsigned int index);
		
		double getMotorBufferAnimation();
		void setMotorbufferAnimation(double time);
		
		
		/**
		 * Simulation
		 */
		void advance(double dt);
		void advanceMotorBuffer(double dt);
		
		
		/**
		 * Drawing
		 */
		void draw(rectd visibleRect);
		
		
		/**
		 * Notifications
		 */
		
		//Location
		virtual void onChangeLocation();
	};
}


#endif
