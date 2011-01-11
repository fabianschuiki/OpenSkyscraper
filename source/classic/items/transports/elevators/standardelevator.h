#ifndef OSS_CLASSIC_ITEMS_TRANSPORTS_ELEVATORS_STANDARDELEVATOR_H
#define OSS_CLASSIC_ITEMS_TRANSPORTS_ELEVATORS_STANDARDELEVATOR_H

#include "elevator.h"


namespace OSS {
	namespace Classic {
		class StandardElevatorItem : public ElevatorItem {
		public:
			static ItemDescriptor descriptor;
			
			//Initialization
			StandardElevatorItem(Tower * tower);
		};
	}
}


#endif
