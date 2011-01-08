#ifndef OSS_STANDARDELEVATOR_H
#define OSS_STANDARDELEVATOR_H


#include "../../general.h"
#include "elevator.h"


namespace OSS {
	class StandardElevatorItem : public ElevatorItem {
	public:
		static Item::Descriptor descriptor;
		
		//Initialization
		StandardElevatorItem(Tower * tower);
	};
}


#endif
