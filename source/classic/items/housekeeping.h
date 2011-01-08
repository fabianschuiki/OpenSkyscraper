#ifndef OSS_HOUSEKEEPING_H
#define OSS_HOUSEKEEPING_H


#include "../../general.h"
#include "../facilityitem.h"
#include "../people/janitor.h"


namespace OSS {
	class HousekeepingItem : public FacilityItem {
	public:
		static Item::Descriptor descriptor;
		
		//Initialization
		HousekeepingItem(Tower * tower);
		void init();
		
		//Basic sprites
		void initBackground();
		
		//Simulation
		void advance(double dt);
		
		//Janitors
	private:
		typedef std::set< Pointer<Janitor> > JanitorSet;
		JanitorSet janitors;
	public:
		void initJanitors();
		void updateJanitors();
		void onJanitorDone(Janitor * janitor);
		
		//Events
		virtual void eventHotelVacated(HotelItem * hotel);
	};
}


#endif
