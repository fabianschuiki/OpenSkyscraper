#ifndef OSS_CLASSIC_ITEMS_TRANSPORTS_ELEVATORS_EXPRESS_H
#define OSS_CLASSIC_ITEMS_TRANSPORTS_ELEVATORS_EXPRESS_H

#include "elevator.h"


namespace OSS {
	namespace Classic {
		class ExpressElevatorItem : public ElevatorItem {
		public:
			static ItemDescriptor descriptor;
			
			//Initialization
			ExpressElevatorItem(Tower * tower);
			virtual string getTypeName() const { return "express"; }
			virtual string getMotorBufferTypeName() const { return "express"; }
		};
	}
}


#endif
