#ifndef OSS_HOTELGUEST_H
#define OSS_HOTELGUEST_H


#include "../../general.h"
#include "timedperson.h"


namespace OSS {
	class HotelItem;
	
	class HotelGuest : public TimedPerson {
	public:
		const Pointer<HotelItem> hotel;
		
		//Initialization
		HotelGuest(Tower * tower, HotelItem * hotel);
		
		//Intelligence
	private:
		bool initialVisitDone;
		bool hadDinner;
		double sleepTime;
		bool didSleep;
	public:
		void think();
		
		//Sleep
	private:
		bool asleep;
	public:
		bool isAsleep();
		void setAsleep(bool asleep);
	};
}


#endif
