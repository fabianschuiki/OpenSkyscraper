#ifndef OSS_CLASSIC_ITEMS_TRANSPORTS_ELEVATORS_STANDARD_H
#define OSS_CLASSIC_ITEMS_TRANSPORTS_ELEVATORS_STANDARD_H

#include "elevator.h"


namespace OSS {
	namespace Classic {
		class StandardElevatorItem : public ElevatorItem {
		public:
			static ItemDescriptor descriptor;
			
			//Initialization
			StandardElevatorItem(Tower * tower);
			virtual string getTypeName() const { return "standard"; }
		};
	}
}


#endif
