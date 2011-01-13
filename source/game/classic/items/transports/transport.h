#ifndef OSS_CLASSIC_ITEMS_TRANSPORTS_TRANSPORT_H
#define OSS_CLASSIC_ITEMS_TRANSPORTS_TRANSPORT_H

#include "../item.h"

#include "../../people/person.h"


namespace OSS {
	namespace Classic {
		class TransportItem : public Item {
		public:
			//Initialization
			TransportItem(Tower * tower, ItemDescriptor * descriptor) : Item(tower, descriptor) {}
			
			//Connection
			virtual bool connectsToFloor(int floor);
			std::set<int> getConnectedLobbies();
			virtual std::set<int> getConnectionFloors();
			
			//Identity
			bool isElevator();
			bool isStairslike();
			bool isStairs();
			bool isEscalator();
		};
	}
}


#endif
