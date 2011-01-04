#ifndef OSS_OFFICE_H
#define OSS_OFFICE_H

#include "../../general.h"
#include "../facilityitem.h"
#include "../route.h"
#include "../timedperson.h"


namespace OSS {
	class OfficeItem : public FacilityItem {
	public:
		static Item::Descriptor descriptor;
		
		//Initialization
	public:
		OfficeItem(Tower * tower);
		
		//Type
	private:
		unsigned int type;
	public:
		unsigned int getType();
		void setType(const unsigned int type);
		
		//Vacancy
	private:
		bool vacant;
	public:
		bool isVacant();
		void setVacant(const bool vacant);
		
		//Basic Sprites
		void updateBackground();
		
		//Simulation
		void advance(double dt);
		void advanceWorkers(double dt);
		void advanceWorker(std::string key, TimedPerson * worker);
		
		//Reachability
	private:
		Pointer<Route> routeFromLobby;
	public:
		Route * getRouteFromLobby() const;
		void setRouteFromLobby(Route * route);
		bool isReachableFromLobby();
		void updateRouteFromLobby();
		
		//Occupancy
		double occupancyTime;
		bool isAttractiveForUse();
		
		//Notifications
		void onChangeLocation();
		void onChangeTransportItems();
		
		//Workers
	private:
		typedef std::map< std::string, Pointer<TimedPerson> > WorkerMap;
		WorkerMap workers;
	public:
		void initWorkers();
		void clearWorkers();
		//void updateWorkerSchedules();
		//void updateSalesmanSchedule(ScheduledPerson * person);
		//void updateWorkerSchedule(ScheduledPerson * person);
	};
}


#endif
