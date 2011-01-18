#ifndef OSS_CLASSIC_ITEMS_TRANSPORTS_ELEVATORS_CAR_H
#define OSS_CLASSIC_ITEMS_TRANSPORTS_ELEVATORS_CAR_H

#include "elevator.h"


namespace OSS {
	namespace Classic {
		class ElevatorCar : public GameObject {
			
			/**
			 * Initialization
			 */
		public:
			const Pointer<ElevatorItem> elevator;
			
			ElevatorCar(ElevatorItem * elevator);
			
			
			/**
			 * Simulation
			 */
		private:
			double floor;
			
			double startFloor;
			int destinationFloor;
			double journeyTime;
			
			Pointer<SoundEffect> arrivingSound;
			Pointer<SoundEffect> departingSound;
			
			bool arrivingPlayed;
			bool departingPlayed;
			
		public:
			double getFloor();
			void setFloor(double f);
			
			int getDestinationFloor();
			void setDestinationFloor(int f);
			
			virtual void advance(double dt);
			
			
			/**
			 * State
			 */
		public:
			unsigned int getTextureSliceIndex();
			
			virtual void update();
			virtual void updateTexture();
			virtual void updatePosition();
			
			Updatable::Conditional<ElevatorCar> updateTextureIfNeeded;
			Updatable::Conditional<ElevatorCar> updatePositionIfNeeded;
			
			
			/**
			 * Drawing
			 */
		private:
			Pointer<Sprite> sprite;
			
		public:
			virtual void draw(rectd dirtyRect);
		};
	}
}


#endif
