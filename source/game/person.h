#ifndef OSS_PERSON_H
#define OSS_PERSON_H


#include "../general.h"
#include "../core/coreobject.h"
#include "item.h"
#include "route.h"


namespace OSS {	
	class Person : public CoreObject {
	protected:
		const Pointer<Tower> tower;
		
	public:
		//Initialization
		Person(Tower * tower);
		
		//Journey
	private:
		int currentFloor;
		Pointer<Item> currentItem;
		Route::Nodes::iterator currentRouteNode;
		int nextFloor;
	public:
		void initJourney();
		void advanceJourney();
		
		
		//Route
	private:
		Pointer<Item> destination;
		Pointer<Route> route;
	public:
		Item * getDestination();
		void setDestination(Item * destination);
		Route * getRoute();
		void setRoute(Route * route);
		void updateRoute();
	};
}


#endif
