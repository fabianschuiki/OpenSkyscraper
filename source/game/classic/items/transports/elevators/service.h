#ifndef OSS_CLASSIC_ITEMS_TRANSPORTS_ELEVATORS_SERVICE_H
#define OSS_CLASSIC_ITEMS_TRANSPORTS_ELEVATORS_SERVICE_H

#include "elevator.h"


namespace OSS {
	namespace Classic {
		class ServiceElevatorItem : public ElevatorItem {
		public:
			static ItemDescriptor descriptor;
			
			//Initialization
			ServiceElevatorItem(Tower * tower);
			virtual string getTypeName() const { return "service"; }
		};
	}
}


#endif
