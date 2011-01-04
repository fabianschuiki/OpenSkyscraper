#ifndef OSS_TIMEDPERSON_H
#define OSS_TIMEDPERSON_H


#include "../general.h"
#include "person.h"


namespace OSS {
	class TimedPerson : public Person {
	public:
		//Initialization
		TimedPerson(Tower * tower) : Person(tower) {}
		
		//Timing
	private:
		Pointer<Item> nextDestination;
		double nextDestinationTime;
		bool nextDestinationValid;
	public:
		Item * getNextDestination();
		double getNextDestinationTime();
		void setNextDestination(double time, Item * destination);
		void clearNextDestination();
		void updateTimedJourney();
		bool hasNextDestination();
	};
}


#endif
