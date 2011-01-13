#ifndef OSS_CLASSIC_ITEMS_TRANSPORTS_ELEVATORS_ELEVATORBACKGROUND_H
#define OSS_CLASSIC_ITEMS_TRANSPORTS_ELEVATORS_ELEVATORBACKGROUND_H

#include "../../../external.h"


namespace OSS {
	namespace Classic {
		class ElevatorItem;
		
		class ElevatorBackground : public Texture {
		public:
			const Pointer<ElevatorItem> elevator;
			
			//Initialization
			ElevatorBackground(ElevatorItem * elevator);
			
			//Required textures
		private:
			Pointer<Texture> backgroundNormal;
			Pointer<Texture> backgroundHighlighted;
			Pointer<Texture> lsNormal;
			Pointer<Texture> lsHighlighted;
			Pointer<Texture> msNormal;
			Pointer<Texture> msHighlighted;
			
			//Update
		private:
			bool needsUpdate;
		public:
			void setNeedsUpdate();
			void update();
			
			void drawFloor(int floor, rectd rect);
			void drawFloorBackground(rectd rect);
			void drawFloorNumber(int floor, rectd rect);
		};
	}
}


#endif
