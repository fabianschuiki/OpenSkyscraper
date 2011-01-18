#ifndef OSS_CLASSIC_ITEMS_TRANSPORTS_ELEVATORS_ELEVATOR_H
#define OSS_CLASSIC_ITEMS_TRANSPORTS_ELEVATORS_ELEVATOR_H

#include "../transport.h"


namespace OSS {
	namespace Classic {
		class ElevatorItem : public TransportItem {
			
			/**
			 * Initialization
			 */
		public:
			ElevatorItem(Tower * tower, ItemDescriptor * descriptor);
			virtual string getMotorBufferTypeName() const { return "standard"; }
			
			
			/**
			 * Layout
			 */
		public:
			bool isFloorActive(int floor);
			bool isFloorHighlighted(int floor);
			
			recti getMotorRect();
			recti getBufferRect();
			recti getCarsRect();
			
			
			/**
			 * Simulation
			 */
		private:
			unsigned int animationFrame;
			
		public:
			unsigned int getAnimationFrame();
			void setAnimationFrame(unsigned int af);
			
			virtual void advanceItem(double dt);
			
			bool shouldAnimate() { return true; }
			
			
			/**
			 * State
			 */
		public:
			virtual void updateBackground();
			
			
			/**
			 * Drawing
			 */
		private:
			Pointer<Sprite> motorSprite;
			Pointer<Sprite> bufferSprite;
			
			//Background textures
			Pointer<Texture> backgroundNormal;
			Pointer<Texture> backgroundHighlighted;
			Pointer<Texture> lsNormal;
			Pointer<Texture> lsHighlighted;
			Pointer<Texture> msNormal;
			Pointer<Texture> msHighlighted;
			
		public:
			virtual void drawItem(rectd dirtyRect);
			virtual void drawBackground(rectd dirtyRect);
			virtual void drawFloor(int f);
			virtual void drawFloorBackground(int f, rectd rect);
			virtual void drawFloorNumber(int f, rectd rect);
			virtual void drawCars(rectd dirtyRect);
		};
	}
}


#endif
