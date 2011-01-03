#ifndef OSS_TRANSPORTITEM_H
#define OSS_TRANSPORTITEM_H

#include "../general.h"
#include "item.h"


namespace OSS {
	class TransportItem : public Item {
	public:
		//Initialization
		TransportItem(Tower * tower, Item::Descriptor * descriptor) : Item(tower, descriptor) {}
		
		//Connection
		virtual bool connectsToFloor(int floor);
		std::set<int> getConnectedLobbies();
		virtual std::set<int> getConnectionFloors() = 0;
		
		//Identity
		bool isElevator();
		bool isStairslike();
		bool isStairs();
		bool isEscalator();
	};
}


#endif
